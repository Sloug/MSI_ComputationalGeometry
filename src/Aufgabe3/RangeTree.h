//
// Created by unclec on 13.12.22.
//

#ifndef COMPUTATIONALGEOMETRY_RANGETREE_H
#define COMPUTATIONALGEOMETRY_RANGETREE_H
#include <QList>
#include <QPointF>

//enum class Direction {horizontal, vertical};
//Direction operator!(const Direction& direction) {
//    switch (direction) {
//        case Direction::horizontal: return Direction::vertical;
//        case Direction::vertical: return Direction::horizontal;
//    }
////    return Direction::horizontal;
//}

class Range {
public:
    Range(double X1, double X2, double Y1, double Y2) {
        x1 = X1;
        x2 = X2;
        y1 = Y1;
        y2 = Y2;
    }
    double x1, x2, y1, y2;
    bool isInRange(QPointF p);
};

class Node {
public:
    QPointF value;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }
};

class RangeTree {
public:
    RangeTree() {
        root = nullptr;
    }
    void partition(bool vertical, int left, int middle, int right);
    void constructBalanced2dTree(QList<QPointF> X, QList<QPointF> Y);
    void rangeSearch(QPointF firstRectangularPointBuffer, QPointF thirdRectangularPointBuffer);
    bool rangeContains(QPointF p);
    Node *root;
private:

    QList<QPointF> X;
    QList<QPointF> Y;
    QList<QPointF> insideRange;
    void constructBalanced2dTree(int leftIndex, int rightIndex, Node *&k, bool vertical);
    void rangeSearch(Node *k, bool vertical, Range D);
    void printXY();
};


#endif //COMPUTATIONALGEOMETRY_RANGETREE_H
