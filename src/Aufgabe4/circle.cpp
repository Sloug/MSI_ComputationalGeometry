//
// Created by unclec on 05.01.23.
//

#include "circle.h"

bool Circle::circleContainsPoint(QPointF p) {
    return sqrt(pow((p.x() - x),2) + pow((p.y() - y),2)) < radius;
}