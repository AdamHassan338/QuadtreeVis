#include "widget.h"
#include "src/ui_widget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QVector2D>
#include <chrono>


const double PUSH_FORCE = 50;
const double DAMPING = .95;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMouseTracking(true);

    setCursor(Qt::CrossCursor);

    root = new Node(QRect(0, 0, this->width(), this->height()),4,nullptr);

    colours.push_back(QColor("#FF0000"));//red
    colours.push_back(QColor("#F2FF00"));//yellow
    colours.push_back(QColor("#00FF19"));//green
    colours.push_back(QColor("#00D8FF"));//turqise
    colours.push_back(QColor("#3300FF"));//BLUE
    colours.push_back(QColor("#C0C0C0"));//grey
    //#2b0080

    connect(&gameLoopTimer, &QTimer::timeout, this, &Widget::gameLoop);
    gameLoopTimer.start(m_tickRate);
    lastUpdateTime = timer.elapsed();


}

void Widget::gameLoop(){
    processInput();
    root->update(circles);
    qint64 currentTime = timer.elapsed();
    double deltaTime = (currentTime - lastUpdateTime) / 1000.0;  // Convert to seconds
    lastUpdateTime = currentTime;
    auto start = std::chrono::high_resolution_clock::now();
    tick(deltaTime);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    qDebug()<< "tick execution time: " << duration.count() << " microseconds, for : " << circles.size() << "Circles";
    if(m_moved)
    update();

}

void Widget::processInput(){
    if (leftMousePressed) {
    Circle* newCircle = new Circle(lastMousePos);
    circles.push_back(newCircle);
    root->insert(newCircle);
    leftMouseReleased = false;
    }

    if (rightMousePressed) {

    }

}

void Widget::tick(qint64 delta){
    Circle mouse = Circle(lastMousePos,20);

    //Interactions with the mouse
    for (Circle* circle : circles) {
        if (circle->contains(lastMousePos) && rightMousePressed) {
            QVector2D pushDirection = QVector2D(circle->pos.x() - lastMousePos.x(), circle->pos.y() - lastMousePos.y());

            // Normalize and apply force
            pushDirection.normalize();
            pushDirection*=10;
            //pushDirection /= distance;
            circle->pos += (pushDirection).toPoint();

            circle->velocity += pushDirection * PUSH_FORCE * delta;
        }
    }
    for(Circle* circle : circles){
        QPoint  pos = circle->pos;
        int rad = circle->rad;
        QRect rect = QRect(pos.x() - rad, pos.y() - rad, 4 * rad, 4 * rad);
        QVector<Circle*> tests = root->contains(rect);

        for (int i = 0; i < tests.size(); ++i) {
                if (circlesCollide(circle, tests[i])) {
                    resolveCollision(circle, tests[i]);
            }
        }

    }
    //checkCollisionsInOctree(root);
/*
    Handle circle-circle collisions
    for (int i = 0; i < circles.size(); ++i) {
        for (int j = i + 1; j < circles.size(); ++j) {
            if (circlesCollide(circles[i], circles[j])) {
                resolveCollision(circles[i], circles[j]);
            }
        }
    }


    //Update positions
    for (Circle* circle : circles) {
        circle->pos += (circle->velocity * delta).toPoint();

        // Apply damping to velocity
        circle->velocity *= DAMPING;
    }*/


}

void Widget::checkCollisionsInOctree(Node *node)
{

    for (int i = 0; i < node->m_circles.size(); ++i) {
        for (int j = i + 1; j < node->m_circles.size(); ++j) {
            if (circlesCollide(node->m_circles[i], node->m_circles[j])) {
                resolveCollision(node->m_circles[i], node->m_circles[j]);
            }
        }
    }


    // Check collisions with neighboring nodes
    QVector<Node*> neighbors = node->getNeighboringNodes();
    for (Circle* circle : node->m_circles) {
        for (Node* neighbor : neighbors) {
            for (Circle* otherCircle : neighbor->m_circles) {
                if (circlesCollide(circle, otherCircle)) {
                    resolveCollision(circle, otherCircle);
                }
            }
        }
    }

    // Recursively check for collisions in child nodes
    for (Node* child : node->m_children) {
        checkCollisionsInOctree(child);
    }
}

Widget::~Widget()
{
    delete ui;
}

QColor sampleColourByDepth(const QVector<QColor>& colours, int depth) {
    if (depth == 0) {
        return colours.last();
    } else if (depth - 1 < colours.size()) {
        return colours[colours.size() - depth];
    } else {
        return colours.last();
    }
}

QVector<Node*> sampleColourBydist(Node* root){
    return root->getNeighboringNodes();
}



void Widget::drawQuadtree(QPainter *painter, Node *node)
{
    if (node) {
        QColor colorForNode = sampleColourByDepth(colours, node->m_currentDepth);
        painter->setBrush(QBrush(colorForNode));
        painter->drawRect(node->m_bounds);
        for (Node* child : node->m_children) {
            drawQuadtree(painter, child);
        }
    }
}

bool Widget::circlesCollide(const Circle *c1, const Circle *c2)
{
    int dx = c1->pos.x() - c2->pos.x();
    int dy = c1->pos.y() - c2->pos.y();
    int combinedRadius = c1->rad + c2->rad;
    return dx * dx + dy * dy <= combinedRadius * combinedRadius;
}

void Widget::resolveCollision(Circle *c1, Circle *c2)
{
    QVector2D collisionDirection = QVector2D(c2->pos.x() - c1->pos.x(), c2->pos.y() - c1->pos.y());
    collisionDirection.normalize();
    double distance = QVector2D(c1->pos-c2->pos).length();

    double overlap = c1->rad + c2->rad - distance;

    // Push away the circles
    c1->pos -= (collisionDirection * (overlap * 0.5)).toPoint();
    c2->pos += (collisionDirection * (overlap * 0.5)).toPoint();
}


void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    drawQuadtree(&painter, root);
    painter.restore();
    painter.save();
    for(Circle* c : circles){
        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter.drawEllipse(c->pos, c->rad, c->rad);
    }
    painter.restore();
}



void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        leftMousePressed = true;
    } else if (event->button() == Qt::RightButton) {
        rightMousePressed = true;
    }
    lastMousePos = event->pos();

}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    lastMousePos = event->pos();
    m_moved = true;
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        leftMousePressed = false;
        leftMouseReleased = true;
    } else if (event->button() == Qt::RightButton) {
        rightMousePressed = false;
        rightMouseReleased = true;
    }
    lastMousePos = event->pos();

}

void Widget::focusInEvent(QFocusEvent *event)
{
    gameLoopTimer.stop();
}

void Widget::focusOutEvent(QFocusEvent *event)
{
    gameLoopTimer.start(m_tickRate);
}

void Widget::resizeEvent(QResizeEvent *event)
{

    root->resize(this->rect());

    QWidget::resizeEvent(event);


    update();
}
