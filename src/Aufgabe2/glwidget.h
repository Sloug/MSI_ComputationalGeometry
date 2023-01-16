//
// GUI-template for CG-assignments
//
// (c) Georg Umlauf, 2014
// (c) Georg Umlauf, 2020: Qt5
// (c) Georg Umlauf, 2022: Qt6
//
#pragma once

#include <QOpenGLWidget>
#include "AffineGeometry.h"
#include "points.h"
#include <QList>
#include "utils.h"
#include "lines.h"
#include "coordLineLink.h"
#include "minHeap.h"
#include "AvlTree.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLWidget                  (QWidget *parent=0);
    ~GLWidget                 ();
signals:
    void continueRequest      ();
public slots:
    void radioButton1Clicked  ();
    void radioButton2Clicked  ();
    void radioButton3Clicked  ();
    void setNum            (int);
    void handleButton         ();
protected:
    void paintGL              ();
    void initializeGL         ();
    void resizeGL             (int width, int height);
    void keyPressEvent        (QKeyEvent   *event);
    void mousePressEvent      (QMouseEvent *event);
private:
    QPointF transformPosition (QPoint p);
    double  aspectx, aspecty;
    int pointsSize;
    bool convexHullFlag;
//    QList<Point> points;
    Lines lines;
    Points points;
    void generatePoints();
    double direction(Vector u, Vector v);
    bool makesLeftTurn(QPointF a, QPointF b, QPointF c);
    QList<QPointF> grahamFunction();
    QList<QPointF> jarvisFunction();
    QList<QPointF> sweepFunction();

    enum Mode { jarvis, graham, sweep };
    Mode mode;
};


