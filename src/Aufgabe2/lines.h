//
// Created by unclec on 17.11.22.
//

#ifndef COMPUTATIONALGEOMETRY_LINES_H
#define COMPUTATIONALGEOMETRY_LINES_H

#include <QList>
#include <QPointF>
#include "utils.h"

class Line {
public:
    Line(float xStart, float yStart, float xEnd, float yEnd);
    float getStartPointX();
    float getStartPointY();
    float getEndPointX();
    float getEndPointY();
    bool isVertical();
private:
    QPointF start;
    QPointF end;
};

class Lines {
public:
    Lines();
    void generateLine();
    void clearLines();
    int getCount();
    float getStartPointX(int   i);
    float getStartPointY(int   i);
    float getEndPointX(int   i);
    float getEndPointY(int   i);
    bool isVertical(int i);
private:
    void addLine(float xStart, float yStart, float xEnd, float yEnd);
    QList<Line> lines;
};




#endif //COMPUTATIONALGEOMETRY_LINES_H
