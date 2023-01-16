//
// Created by unclec on 05.01.23.
//

#ifndef COMPUTATIONALGEOMETRY_CIRCLE_H
#define COMPUTATIONALGEOMETRY_CIRCLE_H
#include <QPointF>

class Circle {
public:
    double radius;
    double x;
    double y;
    bool circleContainsPoint(QPointF p);
};


#endif //COMPUTATIONALGEOMETRY_CIRCLE_H
