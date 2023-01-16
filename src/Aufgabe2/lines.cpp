//
// Created by unclec on 17.11.22.
//

#include "lines.h"

Line::Line(float xStart, float yStart, float xEnd, float yEnd) {
    start = QPointF(xStart, yStart);
    end = QPointF(xEnd, yEnd);
}

float Line::getStartPointX() {
    return start.x();
}
float Line::getStartPointY() {
    return start.y();
}
float Line::getEndPointX() {
    return end.x();
}
float Line::getEndPointY() {
    return end.y();
}
bool Line::isVertical() {
    return start.x() == end.x();
}


Lines::Lines() {
    for (int i = 0; i < 10; ++i) {
        generateLine();
    }
}

void Lines::addLine(float xStart, float yStart, float xEnd, float yEnd){
    lines.append(Line(xStart, yStart, xEnd, yEnd));
}

void Lines::clearLines() {
    lines.clear();
}

int Lines::getCount() {
    return lines.size();
}

float Lines::getStartPointX(int   i) {
    return lines[i].getStartPointX();
}
float Lines::getStartPointY(int   i) {
    return lines[i].getStartPointY();
}
float Lines::getEndPointX(int   i) {
    return lines[i].getEndPointX();
}
float Lines::getEndPointY(int   i) {
    return lines[i].getEndPointY();
}
bool Lines::isVertical(int i) {
    return lines[i].isVertical();
}

void Lines::generateLine() {
    double x = Utils::fRand(-1.0, 1.0);
    double y = Utils::fRand(-1.0, 1.0);
    bool vertical = Utils::bRand();
    double third = Utils::fRand(-1.0, 1.0);
    if (vertical) {
        double otherY = third;
        if (y < otherY) {
            addLine(x, y, x, otherY);
        } else {
            addLine(x, otherY, x, y);
        }
    } else {
        double otherX = third;
        if (x < otherX) {
            addLine(x, y, otherX, y);
        } else {
            addLine(otherX, y, x, y);
        }
    }
}