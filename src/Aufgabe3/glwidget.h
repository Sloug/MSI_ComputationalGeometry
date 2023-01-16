//
// GUI-template for CG-assignments
//
// (c) Georg Umlauf, 2014
// (c) Georg Umlauf, 2020: Qt5
// (c) Georg Umlauf, 2022: Qt6
//
#pragma once

#include <QOpenGLWidget>
#include <QList>
#include "utils.h"
#include "points.h"
#include "RangeTree.h"

typedef std::pair<QPointF, QPointF> lineSeg;
class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLWidget                  (RangeTree *rT, QWidget *parent=0);
    ~GLWidget                 ();
signals:
    void continueRequest      ();
public slots:
    void setNum            (int);
    void handleButton         ();
protected:
    void paintGL              ();
    void initializeGL         ();
    void resizeGL             (int width, int height);
    void keyPressEvent        (QKeyEvent   *event);
    void mousePressEvent      (QMouseEvent *event);
    void mouseReleaseEvent    (QMouseEvent *event);
private:
    bool rectangle;
    QList<QPointF> X;
    QList<QPointF> Y;
    RangeTree *rangeTree;
    std::vector<lineSeg> drawPartitions() const;
    void drawPartitionsR(const double minX,const  double maxX, const double minY, const double maxY, const bool vertical, Node *node,
                                   std::vector<lineSeg> &partitions) const;
    QPointF transformPosition (QPoint p);
    QPointF firstRectangularPointBuffer;
    QPointF secondRectangularPointBuffer;
    QPointF thirdRectangularPointBuffer;
    QPointF fourthRectangularPointBuffer;
    double  aspectx, aspecty;
    int pointsSize;
    Points points;
    void generatePoints();
//    void constructBalanced2dTree(int leftIndex, int rightIndex, Node k, Direction direction);
};


