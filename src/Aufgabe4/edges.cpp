//
// Created by unclec on 03.01.23.
//

#include "edges.h"

Edge::Edge(QPointF start, QPointF end) {
    startPoint = start;
    endPoint = end;
}


void Edges::addEdge(QPointF start, QPointF end) {
    edges.append(Edge(start, end));
}

