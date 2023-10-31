#ifndef NODE_H
#define NODE_H

#include <qwidget.h>
#include "circle.h"

class Node
{
public:
    static const int MAX_DEPTH = 6;
    Node* m_parent;
    int m_currentDepth;
    QRect m_bounds;
    int m_capacity;
    QVector<Circle*> m_circles;
    QVector<Node*> m_children;

    enum quadrant {one,two,three,four};
    enum direction {N,E,S,W,NW,NE,SW,SE};

    Node(QRect bounds, int capacity, Node* parent);
    bool insert(Circle* circle);
    Node* find(Circle* circle);
    QVector<Circle*> contains(QRect area);
    Node* findPos(QPoint p);
    Node* search(QRect rect);
    Node* getNeighbour(quadrant quad);
    void update(QVector<Circle*> circles);

    Node* getBigNeighbour(direction direction);

    QVector<Node*> getSmallNeighbour(Node* neighbor, direction direction);

    QVector<Node*> getNeighboringNodes();

    void subdivide();
    void prune(const QPoint& cursor);
    void resize(QRect newBounds);


    ~Node();
};
#endif // NODE_H
