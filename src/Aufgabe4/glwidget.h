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
#include "AffineGeometry.h"
#include "points.h"
#include "edges.h"
#include "triangles.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLWidget                  (QWidget *parent=0);
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
private:
    QPointF transformPosition (QPoint p);
    double  aspectx, aspecty;
    int pointsSize;
    Points points;
    void generatePoints();
    double fRand(double fMin, double fMax);
    Triangles delaunay(Points p);
    double direction(Vector u, Vector v);
    bool makesLeftTurn(QPointF a, QPointF b, QPointF c);
    QList<QPointF> grahamFunction(Points p);
    QList<QPointF> differenceSet(QList<QPointF> a, QList<QPointF> b);
//    void DrawCircle(float, float, float);
};


