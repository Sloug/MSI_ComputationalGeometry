//
// Created by unclec on 05.12.22.
//

#ifndef COMPUTATIONALGEOMETRY_TREEWIDGET_H
#define COMPUTATIONALGEOMETRY_TREEWIDGET_H
#include <QOpenGLWidget>
#include <QList>
#include "utils.h"
#include "points.h"
#include "RangeTree.h"
#include <cmath>

class TreeWidget : public QOpenGLWidget {
public:
    TreeWidget                  (RangeTree *rangeTree, QWidget *parent=0);
    ~TreeWidget                 ();
protected:
    void paintGL              ();
private:
    RangeTree *rangeTree;
    QList<bool> treeNodes;
    QList<bool> rangeNodes;
    bool printLevel(Node *root, int level);
};


#endif //COMPUTATIONALGEOMETRY_TREEWIDGET_H
