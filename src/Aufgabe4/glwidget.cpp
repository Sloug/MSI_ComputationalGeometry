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
    // clear
//    Triangle tmp = Triangle(QPointF(-0.44,0.1), QPointF(-0.32,0.53), QPointF(0.82,-0.6)); // x: 0.38, y: 0.1, radius: 0.82
//    Circle c = tmp.toCircle();
//    qDebug() << "Circle: X: " << c.x << " Y: " << c.y << " radius: " << c.radius;
//    return;
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
//        qDebug() << "X: " << points.getPointX(i) << ", Y: " << points.getPointY(i);
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

//    qDebug() << "Equal: " << (QPointF(0, 0) == QPointF(0, 0));
//    qDebug() << "Not Equal: " << (QPointF(0, 0) != QPointF(1, 0));
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
//        qDebug() << "Implement mousePressEvent for mous-click-input of points at" <<posF;
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
//    qDebug() << "Calculating Convex Hull";
    QList<QPointF> convexHull;
//    qDebug() << "Sorting";
    p.sortByX();
//    qDebug() << "Calculating Upper Hull";
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
//    qDebug() << "Calculating Lower Hull";
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
//    for (int i = 0; i < convexHull.size(); ++i) {
//        qDebug() << "X: " << convexHull[i].x() << "Y: " << convexHull[i].y();
//    }
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
//    qDebug() << "Calculating 1";
//    qDebug() << p.getCount();
    QList<QPointF> convexHull = grahamFunction(p);
//    Compute a random permutation 𝑝1, ... , 𝑝𝑚 ∈ 𝑃 ∖CH(𝑃) of the remaining points;
    QList<QPointF> randomPermutation = differenceSet(p.getPoints(), convexHull);
    std::random_shuffle(randomPermutation.begin(), randomPermutation.end());
//    Compute an initial triangulation 𝐷 by connecting 𝑝1 to the corners of CH(𝑃);
//    qDebug() << "Zero point: X: " << randomPermutation[0].x() << " Y: " << randomPermutation[0].y();
    for (int i = 1; i < convexHull.size(); ++i) {
        tr.addTriangle(randomPermutation[0], convexHull[i-1], convexHull[i]);
//        output.addEdge(randomPermutation[0], convexHull[i]);
    }
    if(convexHull.size() != 0)
    tr.addTriangle(randomPermutation[0], convexHull[0], convexHull[convexHull.size()-1]);
//    Flip all edges, which are violating the delauney condition
    tr.restoreDelauneyByFLipping();
//    glColor3f(0.0,1.0,0.0);
//    DrawCircle(0.0, 0.0, 1.0);
//    glEnd();
//    glColor3f(0.0,1.0,0.0);
//    glPointSize(7.0);
//    glBegin(GL_POINTS);
//    glVertex2f( randomPermutation[0].x(), randomPermutation[0].y());
//    glColor3f(1.0,1.0,0.0);
//    glVertex2f( convexHull[0].x(), convexHull[0].y());
//    glColor3f(0.0,1.0,1.0);
//    glVertex2f( convexHull[convexHull.size()-1].x(), convexHull[convexHull.size()-1].y());
//    glEnd();
//    qDebug() << "Convex Hull: " << convexHull.size();
//    return tr;

//    for (𝑟 = 2, ... , 𝑚) {
//        Find triangle 𝑝𝑖𝑝𝑗𝑝𝑘 containing 𝑝𝑟;
//        Find all triangles whose Delaunay-conditions is violated, using the adjacency of triangles;
//        Remove these triangles from 𝐷 leaving a polygonal hole 𝛥 in 𝐷.
//        Connect all corners of 𝛥 to 𝑝𝑟 with edges to re-triangulate the hole 𝛥;
//    }
    for (int i = 1; i < randomPermutation.size(); ++i) {
//        qDebug() << "Acutal point: X: " << randomPermutation[i].x() << "Y: " << randomPermutation[i].y();
        // Find triangle 𝑝𝑖𝑝𝑗𝑝𝑘 containing 𝑝𝑟;
//        QList<QPointF> containingTriangle;
//        int containingTriangleIndex;
//        for (int j = 0; j < tr.length(); ++j) {
//            Points cH;
//            cH.addPoint(randomPermutation[i].x(), randomPermutation[i].y());
//            QList<QPointF> trPoints = tr.getPoints(j);
//            for (int k = 0; k < trPoints.size(); ++k) {
//                cH.addPoint(trPoints[k].x(), trPoints[k].y());
//            }
//            qDebug() << "Calculating 2";
//            qDebug() << cH.getCount();
//            QList<QPointF> outputCH = grahamFunction(cH);
//            if(outputCH.size() == 3) {
////                containingTriangle = outputCH;
////                containingTriangleIndex = j;
//                break;
//            }
//        }
        // Find all triangles whose Delaunay-conditions is violated, using the adjacency of triangles;
        Triangles adj;
        QList<int> delauneyBroken;
//        QList<int> delauneyNotBroken;
//        Triangles delauneyBrokenTrangles;
        for (int j = 0; j < tr.length(); ++j) {
//            if(tr.isAdjacentTo(containingTriangleIndex, j) && tr.containsPointInCircle(j, randomPermutation[i]))
            if(tr.containsPointInCircle(j, randomPermutation[i]))
//                delauneyBrokenTrangles.addTriangle(randomPermutation[i]);
                delauneyBroken.append(j);
//            else
//                delauneyNotBroken.append(j);
        }



//        glColor3f(1.0,1.0,0.0);
//        glPointSize(7.0);
//        glBegin(GL_POINTS);
//        glVertex2f( randomPermutation[i].x(), randomPermutation[i].y());
//        glEnd();


        // Remove these triangles from 𝐷 leaving a polygonal hole 𝛥 in 𝐷.
        Points toDelta = tr.distinctPoints(delauneyBroken);


//        if(i==3) {
//            glColor3f(1.0, 1.0, 0.0);
//            glPointSize(7.0);
//            glBegin(GL_POINTS);
//            for (int j = 0; j < toDelta.getCount(); ++j) {
//                glVertex2f(toDelta.getPointX(j), toDelta.getPointY(j));
//            }
//
//
//            glEnd();
//        }


//        qDebug() << "Calculating 3";
//        qDebug() << toDelta.getCount();
//        QList<QPointF> delta;
//        if (i==3)
//            delta = toDelta.clockwise(randomPermutation[i]);
//        else
//            delta = grahamFunction(toDelta);
        QList<QPointF> delta = toDelta.clockwise(randomPermutation[i]);
//        QList<QPointF> delta = grahamFunction(toDelta);;
//        qDebug() << "Calculated";
//        qDebug() << "Before remove: " << tr.length();
//        qDebug() << "To remove: " << delauneyBroken.size();
        tr.remove(delauneyBroken);
//        qDebug() << "After remove: " << tr.length();
//        if(i==2)
//            return tr;
        // Connect all corners of 𝛥 to 𝑝𝑟 with edges to re-triangulate the hole 𝛥;
        Triangles newTriangles;
        for (int j = 1; j < delta.size(); ++j) {
//            qDebug() << "Size: " << delta.size();
//            qDebug() << "J: " << j;
            newTriangles.addTriangle(randomPermutation[i], delta[j-1],  delta[j]);
//            break;
        }
//        delta[delta.size()-1];
        if(delta.size() > 1) {
//            qDebug() << "Adding";
            newTriangles.addTriangle(randomPermutation[i], delta[0], delta[delta.size() - 1]);
        }


//        for (int j = 0; j < delta.size(); ++j) {
//            qDebug() << "Delta: X: " << delta[j].x() << " Y: " << delta[j].y();
//        }
//        for (int j = 0; j < newTriangles.length(); ++j) {
//            QList<QPointF> p = newTriangles.getPoints(j);
//            for (int k = 0; k < p.size(); ++k) {
//                qDebug() << "Second to flip" << k << ": X: " << p[k].x() << " Y: " << p[k].y();
//            }
//        }



        newTriangles.restoreDelauneyByFLipping();
        tr.mergeTriangles(newTriangles);
//        return tr;
//        if(i==3)
//            break;
    }
//    qDebug() << "Calculated!!!!";
    return tr;
}
