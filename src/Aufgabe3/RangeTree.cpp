//
// Created by unclec on 13.12.22.
//

#include "RangeTree.h"
#include "points.h"
#include "qdebug.h"

bool Range::isInRange(QPointF p) {
    return x1 <= p.x() && x2 >= p.x() && y2 <= p.y() && y1 >= p.y();
}

void RangeTree::constructBalanced2dTree(QList<QPointF> X, QList<QPointF> Y) {
    this->X = X;
    this->Y = Y;
    root = nullptr;
    constructBalanced2dTree(0, X.size() - 1, root, true);
}

void RangeTree::constructBalanced2dTree(int leftIndex, int rightIndex, Node *&k, bool vertical) {
//    printXY();
    if (leftIndex <= rightIndex) {
        int m = (leftIndex + rightIndex) / 2; //Todo: toInt
        if (k == nullptr)
            k = new Node();
        if (vertical) {
//            qDebug() << "Ks X is: " << Y[m].x() << "ks Y is: " << Y[m].y();
            k->value = Y[m];
            partition(vertical, leftIndex, m, rightIndex);
        } else {
//            qDebug() << "Ks X is: " << X[m].x() << "ks Y is: " << X[m].y();
            k->value = X[m];
            partition(vertical, leftIndex, m, rightIndex);
        }
        constructBalanced2dTree(leftIndex, m-1, k->left, !vertical);
        constructBalanced2dTree(m+1, rightIndex, k->right, !vertical);
    }
}

void RangeTree::partition(bool vertical, int left, int middle, int right) {
    int c=0;
    QList<QPointF> tmp1, tmp2;
    QList<QPointF> &Z = (vertical) ? X : Y;
//    int j = (!vertical) ? 1 : 0;
    QPointF mpt = (vertical) ? Y[middle] : X[middle];
    for (int i = left; i <= right; ++i) {
        if (!vertical) {
            if (Z[i].x() < mpt.x())
                tmp1.append(Z[i]);
            if (Z[i].x() > mpt.x())
                tmp2.append(Z[i]);
            if (Z[i].x() == mpt.x())
                c++;
            if (c>1) qDebug() << "WARNING MULTIPLE X?Y-COORDINATE";
        } else {
            if (Z[i].y() < mpt.y())
                tmp1.append(Z[i]);
            if (Z[i].y() > mpt.y())
                tmp2.append(Z[i]);
            if (Z[i].y() == mpt.y())
                c++;
            if (c>1) qDebug() << "WARNING MULTIPLE X?Y-COORDINATE";
        }

    }
    Z[middle] = mpt;
    for (int i = 0; i < tmp1.size(); ++i) {
        Z[left + i] = tmp1[i];
    }
    for (int i = 0; i < tmp2.size(); ++i) {
        Z[middle + i + 1] = tmp2[i];
    }
}

void RangeTree::rangeSearch(QPointF firstRectangularPointBuffer, QPointF thirdRectangularPointBuffer) {
    // ol, ul, ur, or
//    qDebug() << "do range search outer";
    insideRange.clear();
    double x1 = firstRectangularPointBuffer.x();
    double y1 = firstRectangularPointBuffer.y();
    double x2 = thirdRectangularPointBuffer.x();
    double y2 = thirdRectangularPointBuffer.y();
    Range D = Range(x1, x2, y1, y2);
    rangeSearch(root, true, D);
    qDebug() << "done range search outer, size is: " << insideRange.size();
}

void RangeTree::rangeSearch(Node *k, bool vertical, Range D) {
//    qDebug() << "do range search";
    if (k != nullptr) {
        double left, right, coord;
//        bool dNew;
        if (vertical) {
            left = D.y2;
            right = D.y1;
            coord = k->value.y();
//            dNew = false;
        } else {
            left = D.x1;
            right = D.x2;
            coord = k->value.x();
//            dNew = true;
        }
        if (D.isInRange(k->value)) {
//            qDebug() << "do range search append";
            insideRange.append(k->value);
        }
        if (left < coord) {
            rangeSearch(k->left, !vertical, D);
        }
        if (right > coord) {
            rangeSearch(k->right, !vertical, D);
        }
    }
}
bool RangeTree::rangeContains(QPointF p) {

    bool r = false;
    for (int i = 0; i < insideRange.size(); ++i) {
//        if ((p.x() == root->value.x()) && (p.y() == root->value.y()))
//        qDebug() << "Inside range x is: " << insideRange[i].x() << "Inside range y is: " << insideRange[i].y();
        if ((p.x() == insideRange[i].x()) && (p.y() == insideRange[i].y())) {
//            qDebug() << "inside";
            r = true;
        }
    }
//    qDebug() << "X is: " << p.x() << "Y is: " << p.y();
//    qDebug() << "Root X is: " << root->value.x() << "Root Y is: " << root->value.y();
//    qDebug() << "Root contains is: " << ((p.x() == root->value.x()) && (p.y() == root->value.y()));
//    qDebug() << "Contains is: " << r;
    return r;
}
void RangeTree::printXY() {
    for (int i = 0; i < X.size(); ++i) {
        qDebug() << "XX is: " << X[i].x() << "XY is: " << X[i].y();
    }
    for (int i = 0; i < Y.size(); ++i) {
        qDebug() << "YX is: " << Y[i].x() << "YY is: " << Y[i].y();
    }
}
