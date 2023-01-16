//
// Created by unclec on 03.01.23.
//

#ifndef COMPUTATIONALGEOMETRY_EDGES_H
#define COMPUTATIONALGEOMETRY_EDGES_H

#include <QPointF>
#include <QList>

class Edge {
public:
    Edge(QPointF start, QPointF end);
    QPointF getStartPoint;
    QPointF getEndPoint;
private:
    QPointF startPoint;
    QPointF endPoint;
};

class Edges {
public:
    void addEdge(QPointF start, QPointF end);
private:
    QList<Edge> edges;
};


#endif //COMPUTATIONALGEOMETRY_EDGES_H
