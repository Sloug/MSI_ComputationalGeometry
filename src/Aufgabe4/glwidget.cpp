//
// GUI-template for CG-assignments
//
// (c) Georg Umlauf, 2014
// (c) Georg Umlauf, 2020: Qt5
// (c) Georg Umlauf, 2022: Qt6
//
#include "glwidget.h"
#include <QtGui>
#include <GL/glu.h>
#include <iostream>
#include "mainwindow.h"
#include <QDebug>


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    pointsSize = 10;
    generatePoints();
}

GLWidget::~GLWidget()
{
}

typedef std::pair<QPointF, QPointF> lineSeg;
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Koordinatensystem
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINES);
    glVertex2f(-1.0, 0.0);
    glVertex2f( 1.0, 0.0);
    glVertex2f( 0.0,-1.0);
    glVertex2f( 0.0, 1.0);

    glEnd();


    // Punkte
    glColor3f(1.0,0.0,0.0);
    glPointSize(7.0);
    glBegin(GL_POINTS);
    for (int i=0; i<points.getCount(); i++) {
        glVertex2f(points.getPointX(i),points.getPointY(i));
    }

    glEnd();

    Triangles triangles = delaunay(points);
    QList<lineSeg> p = triangles.printingPoints();

    // Triangulationsdreiecke
    glBegin(GL_LINES);
    glColor3f(0.0,0.0,1.0);
    for (int i = 0; i < p.size(); ++i) {
        QPointF first = p[i].first;
        QPointF second = p[i].second;
        glVertex2f(first.x(), first.y());
        glVertex2f(second.x(), second.y());
    }
    glEnd();

    //Umkreise
//    glColor3f(0.0,1.0,1.0);
//    for (int i = 0; i < triangles.length(); ++i) {
//        triangles.DrawCircle(i);
//    }

}


void GLWidget::initializeGL()
{
    resizeGL(width(),height());
}

void GLWidget::resizeGL(int width, int height)
{
    aspectx=1.0;
    aspecty=1.0;
    if (width>height) aspectx = float(width) /height;
    else              aspecty = float(height)/ width;
    glViewport    (0,0,width,height);
    glMatrixMode  (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D    (-aspectx,aspectx,-aspecty,aspecty);
    glMatrixMode  (GL_MODELVIEW);
    glLoadIdentity();
}

QPointF GLWidget::transformPosition(QPoint p)
{
    return QPointF( (2.0*p.x()/ width() - 1.0)*aspectx,
                    -(2.0*p.y()/height() - 1.0)*aspecty);
}

void GLWidget::keyPressEvent(QKeyEvent * event)
{
    switch (event->key()) {
        case Qt::Key_Escape:   QApplication::instance()->quit(); break;
        case Qt::Key_Q     :   QApplication::instance()->quit(); break;
        default:               QWidget::keyPressEvent(event);    break;
    }
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    QPointF posF = transformPosition(event->pos());
    if (event->buttons() & Qt::LeftButton ) {
        points.addPoint(posF.x(), posF.y());
    }
    update();
}

void GLWidget::setNum(int num)
{
    pointsSize = num;
}

void GLWidget::handleButton()
{
    generatePoints();
    update();
}

double GLWidget::fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void GLWidget::generatePoints() {
    points.clearPoints();
    for (int i = 0; i < pointsSize; ++i) {
        double x = fRand(-1.0, 1.0);
        double y = fRand(-1.0, 1.0);
        points.addPoint(x, y);
    }
}

QList<QPointF> GLWidget::grahamFunction(Points p) {
    QList<QPointF> convexHull;
    p.sortByX();
    QList<QPointF> upper;
    if(p.getCount() < 3) {
        for (int i = 0; i < p.getCount(); ++i) {
            upper.append(QPointF(p.getPointX(i), p.getPointY(i)));
        }
        return upper;
    }
    upper.append(QPointF(p.getPointX(0), p.getPointY(0)));
    upper.append(QPointF(p.getPointX(1), p.getPointY(1)));
    for (int i = 2; i < p.getCount(); ++i) {
        upper.append(QPointF(p.getPointX(i), p.getPointY(i)));
        int upperSize = upper.size();
        while (upperSize > 2 && makesLeftTurn(upper[upperSize-3], upper[upperSize-2], upper[upperSize-1])) { // upper containes more than two points and the last three points a,b,c make a left turn
            upper.removeAt(upperSize-2); // delete b
            upperSize = upper.size();
        }
    }
    QList<QPointF> lower;
    lower.append(QPointF(p.getPointX(p.getCount() - 1), p.getPointY(p.getCount() - 1)));
    lower.append(QPointF(p.getPointX(p.getCount() - 2), p.getPointY(p.getCount() - 2)));
    for (int i = p.getCount() - 3; i >= 0; --i) {
        lower.append(QPointF(p.getPointX(i), p.getPointY(i)));
        int lowerSize = lower.size();
        while (lowerSize > 2 && makesLeftTurn(lower[lowerSize-3], lower[lowerSize-2], lower[lowerSize-1])) { // upper containes more than two points and the last three points a,b,c make a left turn
            lower.removeAt(lowerSize-2); // delete b
            lowerSize = lower.size();
        }
    }
    lower.removeFirst();
    lower.removeLast();
    upper.append(lower);
    convexHull = upper;
    return convexHull;
}

double GLWidget::direction(Vector u, Vector v) {
    return u ^ v;
}

bool GLWidget::makesLeftTurn(QPointF a, QPointF b, QPointF c) {
    // von https://algorithmtutor.com/Computational-Geometry/Determining-if-two-consecutive-segments-turn-left-or-right/
    Point ap = Point(a.x(), a.y());
    Point bp = Point(b.x(), b.y());
    Point cp = Point(c.x(), c.y());

    Vector u = ap - bp;
    Vector v = bp - cp;
    double dir = direction(u, v);
    return dir > 0; // ist hier andersrum ,da a - b & b - c statt b - a & c - b

}
QList<QPointF> GLWidget::differenceSet(QList<QPointF> a,QList<QPointF> b) {
//    Calculates Set A - Set B
    QList<QPointF> output;
    for (int i = 0; i < a.size(); ++i) {
        if(!b.contains(a[i]))
            output.append(a[i]);
    }
    return output;
}



Triangles GLWidget::delaunay(Points p) {
//    Input: Set 𝑃 of 𝑛 distinct points in the plane.
//    Output: Delaunay-Triangulation 𝐷.
//    Edges output;
    Triangles tr;


//    Compute the convex hull CH(𝑃) of 𝑃;
    QList<QPointF> convexHull = grahamFunction(p);
//    Compute a random permutation 𝑝1, ... , 𝑝𝑚 ∈ 𝑃 ∖CH(𝑃) of the remaining points;
    QList<QPointF> randomPermutation = differenceSet(p.getPoints(), convexHull);
    std::random_shuffle(randomPermutation.begin(), randomPermutation.end());
//    Compute an initial triangulation 𝐷 by connecting 𝑝1 to the corners of CH(𝑃);
    for (int i = 1; i < convexHull.size(); ++i) {
        tr.addTriangle(randomPermutation[0], convexHull[i-1], convexHull[i]);
    }
    if(convexHull.size() != 0)
    tr.addTriangle(randomPermutation[0], convexHull[0], convexHull[convexHull.size()-1]);
//    Flip all edges, which are violating the delauney condition
    tr.restoreDelauneyByFLipping();

//    for (𝑟 = 2, ... , 𝑚) {
//        Find triangle 𝑝𝑖𝑝𝑗𝑝𝑘 containing 𝑝𝑟;
//        Find all triangles whose Delaunay-conditions is violated, using the adjacency of triangles;
//        Remove these triangles from 𝐷 leaving a polygonal hole 𝛥 in 𝐷.
//        Connect all corners of 𝛥 to 𝑝𝑟 with edges to re-triangulate the hole 𝛥;
//    }
    for (int i = 1; i < randomPermutation.size(); ++i) {
        // Find triangle 𝑝𝑖𝑝𝑗𝑝𝑘 containing 𝑝𝑟;

        // Find all triangles whose Delaunay-conditions is violated, using the adjacency of triangles;
        Triangles adj;
        QList<int> delauneyBroken;
        for (int j = 0; j < tr.length(); ++j) {
            if(tr.containsPointInCircle(j, randomPermutation[i]))
                delauneyBroken.append(j);
        }

        // Remove these triangles from 𝐷 leaving a polygonal hole 𝛥 in 𝐷.
        Points toDelta = tr.distinctPoints(delauneyBroken);

        QList<QPointF> delta = toDelta.clockwise(randomPermutation[i]);

        tr.remove(delauneyBroken);

        // Connect all corners of 𝛥 to 𝑝𝑟 with edges to re-triangulate the hole 𝛥;
        Triangles newTriangles;
        for (int j = 1; j < delta.size(); ++j)
            newTriangles.addTriangle(randomPermutation[i], delta[j-1],  delta[j]);

        if(delta.size() > 1) {
            newTriangles.addTriangle(randomPermutation[i], delta[0], delta[delta.size() - 1]);
        }

        tr.mergeTriangles(newTriangles);
    }
    return tr;
}
