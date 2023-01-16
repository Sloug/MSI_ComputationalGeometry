//
// Created by unclec on 04.01.23.
//

#ifndef COMPUTATIONALGEOMETRY_TRIANGLES_H
#define COMPUTATIONALGEOMETRY_TRIANGLES_H

//#include "edges.h"
#include <QPointF>
#include <QList>
#include "circle.h"
#include "points.h"
#include <QDebug>
#include <GL/glu.h>



class Triangle {
public:
    Triangle(QPointF a, QPointF b, QPointF c);
    QPointF getA();
    QPointF getB();
    QPointF getC();
    bool isAdjacent(Triangle target);
    bool hasAdjacentEdge(Triangle target);
    QList<QPointF> getDistinctPoints(Triangle target);
    QPointF getDistinctPoint(Triangle target);
    Circle toCircle();
private:
    QPointF a;
    QPointF b;
    QPointF c;
    bool hasVertex(QPointF p);
};

class Triangles {
public:
    void DrawCircle(int i);
    void addTriangle(QPointF a, QPointF b, QPointF c);
//    void addTriangle(QPointF a, QPointF b, QPointF c);
    int length();
    QList<QPointF> getPoints(int i);
    bool isAdjacentTo(int source, int target);
    bool containsPointInCircle(int i, QPointF p);
    void remove(QList<int> indexes);
    Points distinctPoints(QList<int> indexes);
    void restoreDelauneyByFLipping();
    typedef std::pair<QPointF, QPointF> lineSeg;
    QList<lineSeg> printingPoints();
    void mergeTriangles(Triangles tr);
private:
    QList<Triangle> triangles;
    void DrawCircle(float cx, float cy, float r);
    bool hasAdjacentEdgeTo(int source, int target);
};
#endif //COMPUTATIONALGEOMETRY_TRIANGLES_H
