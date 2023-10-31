#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QElapsedTimer>
#include <QTimer>
#include "node.h"



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    QTimer gameLoopTimer;
    QElapsedTimer timer;
    qint64 lastUpdateTime;
    int m_tickRate = 10;

    bool leftMousePressed = false;
    bool rightMousePressed = false;


    bool leftMouseReleased = false;
    bool rightMouseReleased = false;
    QPoint lastMousePos;
    bool m_moved = false;

    Node* root;
    int maxDepth = 0;
    void drawQuadtree(QPainter* painter, Node* node);
    QVector<QColor> colours;

    QVector<Circle*> circles;

    void gameLoop();

    void processInput();

    void tick(qint64 delta);

    void checkCollisionsInOctree(Node* node);

    bool circlesCollide(const Circle* c1, const Circle* c2);

    void resolveCollision(Circle* c1, Circle* c2);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void focusInEvent(QFocusEvent *event) override;

    void focusOutEvent(QFocusEvent *event) override;
};
#endif // WIDGET_H
