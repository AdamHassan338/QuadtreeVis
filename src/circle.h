#ifndef CIRCLE_H
#define CIRCLE_H


#include <QPoint>
#include <QVector2D>
class Circle {
public:
    Circle(QPoint position, int radius = 5);

    bool contains(const QPoint& point) const;

    QPoint pos;
    int rad;
    QVector2D velocity;
};

#endif // CIRCLE_H
