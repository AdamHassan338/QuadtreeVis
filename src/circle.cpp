#include "circle.h"

Circle::Circle(QPoint position, int radius):
    pos(position), rad(radius) {

    velocity = QVector2D();
}



bool Circle::contains(const QPoint &point) const
{
    int dx = point.x() - pos.x();
    int dy = point.y() - pos.y();
    return dx * dx + dy * dy <= rad * rad;
}
