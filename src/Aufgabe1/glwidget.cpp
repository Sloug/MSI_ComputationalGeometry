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



GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    pointsSize = 10;
    graham = false;
    generatePoints();
}

GLWidget::~GLWidget()
{
}

void GLWidget::paintGL()
{
    // clear
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

    // Test cases left turn
//    qDebug() << makesLeftTurn(QPointF(0, 0), QPointF(0, 1), QPointF(1, 1)); // false x
//    qDebug() << makesLeftTurn(QPointF(0, 0), QPointF(0, 1), QPointF(0, 2)); // false
//    qDebug() << makesLeftTurn(QPointF(0, 0), QPointF(0, 1), QPointF(1, 2)); // false x
//    qDebug() << makesLeftTurn(QPointF(0, 0), QPointF(0, 1), QPointF(-1, 2)); // true x

    // Test cosTheta
//    qDebug() << Vector(0, 1).cosTheta(Vector(1, 0)); // 0
//    qDebug() << Vector(0, 1).cosTheta(Vector(0, 1)); // 1
//    qDebug() << Vector(0, 1).cosTheta(Vector(1, 1)); // 0.707
//    qDebug() << Vector(0, 1).cosTheta(Vector(-1, 1)); // 0.707

    // TODO: Compute and draw convex hull here

    QList<QPointF> convexHull;

    if (graham) { // Graham's scan
        points.sortByX();
        QList<QPointF> upper;
        upper.append(QPointF(points.getPointX(0), points.getPointY(0)));
        upper.append(QPointF(points.getPointX(1), points.getPointY(1)));
        for (int i = 2; i < points.getCount(); ++i) {
            upper.append(QPointF(points.getPointX(i), points.getPointY(i)));
            int upperSize = upper.size();
            while (upperSize > 2 && makesLeftTurn(upper[upperSize-3], upper[upperSize-2], upper[upperSize-1])) { // upper containes more than two points and the last three points a,b,c make a left turn
                upper.removeAt(upperSize-2); // delete b
                upperSize = upper.size();
            }
        }

        QList<QPointF> lower;
        lower.append(QPointF(points.getPointX(points.getCount() - 1), points.getPointY(points.getCount() - 1)));
        lower.append(QPointF(points.getPointX(points.getCount() - 2), points.getPointY(points.getCount() - 2)));
        for (int i = points.getCount() - 3; i >= 0; --i) {
            lower.append(QPointF(points.getPointX(i), points.getPointY(i)));
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

    } else { // TODO: Jarvis' march
        QList<int> allreadyVisited;
        int index_p = points.getMinYX();
        QPointF p = QPointF(points.getPointX(index_p), points.getPointY(index_p));
        convexHull.append(p);
        int i = 2;

        // end point of line p qi with smallest angle to horizontal line through p1
        Point p1 = Point(p.x(), p.y());
        Vector horizonThrougP = p1 - Point(p.x() + 1, p.y());

        double maxCosTheta = 0;
        int smallesAngle = index_p;
        for (int j = 0; j < points.getCount(); ++j) {
            if (j != index_p) {
                Vector v = p1 - Point(points.getPointX(j), points.getPointY(j));
                double cosTheta = horizonThrougP.cosTheta(v);
                if (cosTheta > maxCosTheta) {
                    maxCosTheta = cosTheta;
                    smallesAngle = j;
                }
            }
        }
        QPointF smallestAngleToHorizontal = QPointF(points.getPointX(smallesAngle), points.getPointY(smallesAngle));
        convexHull.append(smallestAngleToHorizontal);
        allreadyVisited.append(smallesAngle);

        do {
            double minCosTheta = 1;
            int flatestAngle = 0;
            i++;
            // alle Punkte die nicht Teil der konvexen Huelle sind, ausser p, durchsuchen.
            for (int j = 0; j < points.getCount(); ++j) {
                if (!allreadyVisited.contains(j) && (i!=3 || j != index_p)) { // fuer i == 3 darf p nicht in der durchsuchten Menge vorkommen
//                    qDebug() << i;
//                    qDebug() << convexHull.size();
                    Point a = Point(convexHull[i-3].x(), convexHull[i-3].y());
                    Point b = Point(convexHull[i-2].x(), convexHull[i-2].y());
//                    Point a;
//                    Point b;
                    Point c = Point(points.getPointX(j), points.getPointY(j));

//                    qDebug() << "Point a:" << convexHull[i-3].x() << convexHull[i-3].y();
//                    qDebug() << "Point b:" << convexHull[i-2].x() << convexHull[i-2].y();
//                    qDebug() << "Point c:" << points.getPointX(j) << points.getPointY(j);


                    Vector u = b - a;
                    Vector v = b - c;
                    double cosTheta = u.cosTheta(v);
//                    qDebug() << "Allready Visited" << allreadyVisited;
//                    qDebug() << allreadyVisited.contains(j);
//                    qDebug() << "j" << j;
//                    qDebug() << "i" << i;
//                    qDebug() << "cosTheta" << cosTheta;
                    if (cosTheta < minCosTheta) {
                        minCosTheta = cosTheta;
                        flatestAngle = j;
                        qDebug() << "Point a:" << convexHull[i-3].x() << convexHull[i-3].y();
                        qDebug() << "Point b:" << convexHull[i-2].x() << convexHull[i-2].y();
                        qDebug() << "Point c:" << points.getPointX(j) << points.getPointY(j);
                        qDebug() << "cosTheta" << cosTheta;
                    }
                }
            }
            QPointF flatestAngleToPrevious = QPointF(points.getPointX(flatestAngle), points.getPointY(flatestAngle));
            convexHull.append(flatestAngleToPrevious);
            allreadyVisited.append(flatestAngle);

        } while (convexHull[i - 1] != convexHull[0]);

    }


    // draw convex hull using glBegin(GL_LINE_STRIP); ... glEnd();

    // Konvexe Huelle
    glColor3f(0.0,1.0,0.0);

    glBegin(GL_LINES);
    for (int i=0; i<convexHull.size()-1; i++) {
        glVertex2f(convexHull[i].x(),convexHull[i].y());
        glVertex2f(convexHull[i+1].x(),convexHull[i+1].y());
    }
    if (convexHull.size() > 2) {
        glVertex2f(convexHull[convexHull.size()-1].x(),convexHull[convexHull.size()-1].y());
        glVertex2f(convexHull[0].x(),convexHull[0].y());
    }


    glEnd();
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


void GLWidget::radioButton1Clicked()
{
    graham = false;
    update();
}

void GLWidget::radioButton2Clicked()
{
    graham = true;
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
