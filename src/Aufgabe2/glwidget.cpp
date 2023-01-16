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
    mode = sweep;
    convexHullFlag = false;
    generatePoints();
}

GLWidget::~GLWidget()
{
}

void GLWidget::paintGL()
{
    cout << "New Painting" << endl;
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


    if (convexHullFlag) { // Convex Hull
        // Punkte
        glColor3f(1.0,0.0,0.0);
        glPointSize(7.0);
        glBegin(GL_POINTS);
        for (int i=0; i<points.getCount(); i++) {
            glVertex2f(points.getPointX(i),points.getPointY(i));
        }

        glEnd();
    } else {
        // Linien

        //Start- und Endpunkte
        glColor3f(1.0,0.0,0.0);
        glPointSize(3.0);
        glBegin(GL_POINTS);
        for (int i=0; i<lines.getCount(); i++) {
            glVertex2f(lines.getStartPointX(i),lines.getStartPointY(i));
            glVertex2f(lines.getEndPointX(i),lines.getEndPointY(i));
        }
        glEnd();

        // Liniensegmente
        glColor3f(0.0,1.0,0.0);

        glBegin(GL_LINES);
        for (int i=0; i<lines.getCount(); i++) {
            glVertex2f(lines.getStartPointX(i),lines.getStartPointY(i));
            glVertex2f(lines.getEndPointX(i),lines.getEndPointY(i));
        }
        glEnd();
    }



    // Compute and draw convex hull or sweep line here


    QList<QPointF> processedPoints;
    switch (mode) {
        case graham : convexHullFlag = true; processedPoints = grahamFunction(); break;
        case jarvis : convexHullFlag = true; processedPoints = jarvisFunction(); break;
        case sweep : convexHullFlag = false; processedPoints = sweepFunction(); break;
    }

    if (convexHullFlag) { // Convex Hull
        // draw convex hull using glBegin(GL_LINE_STRIP); ... glEnd();

        // Konvexe Huelle
        glColor3f(0.0,1.0,0.0);

        glBegin(GL_LINES);
        for (int i=0; i < processedPoints.size() - 1; i++) {
            glVertex2f(processedPoints[i].x(), processedPoints[i].y());
            glVertex2f(processedPoints[i + 1].x(), processedPoints[i + 1].y());
        }
        if (processedPoints.size() > 2) {
            glVertex2f(processedPoints[processedPoints.size() - 1].x(), processedPoints[processedPoints.size() - 1].y());
            glVertex2f(processedPoints[0].x(), processedPoints[0].y());
        }


        glEnd();

    } else { // Sweep Line
        //Schnittpunkte
        glColor3f(0.0,0.0,1.0);
        glPointSize(5.0);
        glBegin(GL_POINTS);
        for (int i=0; i<processedPoints.size(); i++) {
            glVertex2f(processedPoints[i].x(), processedPoints[i].y());
        }
        glEnd();

    }



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
    mode = jarvis;
    convexHullFlag = true;
    update();
}

void GLWidget::radioButton2Clicked()
{
    mode = graham;
    convexHullFlag = true;
    update();
}

void GLWidget::radioButton3Clicked()
{
    mode = sweep;
    convexHullFlag = false;
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


void GLWidget::generatePoints() { // generate lines or Points

    points.clearPoints();
    for (int i = 0; i < pointsSize; ++i) {
        double x = Utils::fRand(-1.0, 1.0);
        double y = Utils::fRand(-1.0, 1.0);
        points.addPoint(x, y);
    }

    lines.clearLines();
    for (int i = 0; i < pointsSize; ++i) {
        lines.generateLine();
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

QList<QPointF> GLWidget::grahamFunction() {
    QList<QPointF> convexHull;

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
    return convexHull;
}

QList<QPointF> GLWidget::jarvisFunction() {
    QList<QPointF> convexHull;

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
                Point a = Point(convexHull[i-3].x(), convexHull[i-3].y());
                Point b = Point(convexHull[i-2].x(), convexHull[i-2].y());

                Point c = Point(points.getPointX(j), points.getPointY(j));

                Vector u = b - a;
                Vector v = b - c;
                double cosTheta = u.cosTheta(v);

                if (cosTheta < minCosTheta) {
                    minCosTheta = cosTheta;
                    flatestAngle = j;
                }
            }
        }
        QPointF flatestAngleToPrevious = QPointF(points.getPointX(flatestAngle), points.getPointY(flatestAngle));
        convexHull.append(flatestAngleToPrevious);
        allreadyVisited.append(flatestAngle);

    } while (convexHull[i - 1] != convexHull[0]);
    return convexHull;
}

QList<QPointF> GLWidget::sweepFunction() {
    QList<QPointF> intersections;
    // Initialize set Q with all events sorted by x-coordinate;
    QList<CoordLineLink> qAsList = CoordLineLink::linesToQ(lines);
    MinHeap<CoordLineLink> Q = MinHeap(qAsList);
    // L = ∅; // List of active segments, sorted by y-coordinate
    AvlTree<CoordLineLink> L;
    AvlTree<int> test;

    while (Q.size() > 0) {
        // Take next event p from Q and remove it from Q;
        CoordLineLink event = Q.top();
        Q.pop();
        // if (p is left end point of horizontal segment s) then
        if (event.isStart()) {
//            cout << "Start: " << event.getCoordinate() << endl;
            // Add s to L;
//            test.insert(1);
            L.insert(CoordLineLink(event.getIndex(), lines.getStartPointY(event.getIndex())));
            cout << "Inserting: " << lines.getStartPointY(event.getIndex()) << endl;
        // else if (p is right end point of segment s) then
        } else if(event.isEnd()) {
//            cout << "End: " << event.getCoordinate() << endl;
            // Remove s from L;
//            cout << "Remove: " << event.getIndex() << endl;
            double test = lines.getStartPointY(event.getIndex());
            L.remove(CoordLineLink(event.getIndex(), lines.getStartPointY(event.getIndex())));
            cout << "Removing: " << lines.getStartPointY(event.getIndex()) << endl;
        // p.key is the x-coordinate of a vertical
        // segment s with lower end point (p.key,yl)
        // and upper end point (p.key,yu)
        } else {
//            cout << "Vertical: " << event.getCoordinate() << endl;
            // Determine all horizontal segments t in L, whose
            // y-coordinate t.y is in [yl,yu] and report the
            // intersecting pair (t,s);
            double yl = lines.getStartPointY(event.getIndex());
            double yu = lines.getEndPointY(event.getIndex());
//            CoordLineLink test = L.search(CoordLineLink(event.getIndex(), lines.getStartPointY(event.getIndex())))->d;
            Node<CoordLineLink> *startNode = L.findNodeWithSmallestKeyGreaterEqual(CoordLineLink(event.getIndex(), yl));
//            if ((startNode != NULL) && (startNode->d.getCoordinate() <= yu) && (startNode->d.getCoordinate() >= yl)) {
            if ((startNode != NULL) && (startNode->key.getCoordinate() <= yu)) {
                intersections.append(QPointF(event.getCoordinate(), startNode->key.getCoordinate()));
                cout << "Intersect: " << startNode->key.getIndex() << endl;
//                while ((startNode->successor != NULL) && (startNode->successor->d.getCoordinate() <= yu) && (startNode->successor->d.getCoordinate() >= yl)) {
                while ((startNode->next != NULL) && (startNode->next->key.getCoordinate() <= yu)) {
                    intersections.append(QPointF(event.getCoordinate(), startNode->next->key.getCoordinate()));
                    cout << "Intersect: " << startNode->key.getIndex() << endl;
                    startNode = startNode->next;
                }
            }
        }
    }

//    Input: n iso-oriented line segments.
//    Output: All intersections.
//    Initialize set Q with all events sorted by x-coordinate;
//    L = ∅; // List of active segments, sorted by y-coordinate
//    while (Q is not empty) {
//        Take next event p from Q and remove it from Q;
//        if (p is left end point of horizontal segment s) then {
//                Add s to L;
//        } else if (p is right end point of segment s) then {
//                Remove s from L;
//        } else { // p.key is the x-coordinate of a vertical
//                 // segment s with lower end point (p.key,yl)
//                 // and upper end point (p.key,yu)
//                Determine all horizontal segments t in L, whose
//                y-coordinate t.y is in [yl,yu] and report the
//                intersecting pair (t,s);
//            }
//        }
    return intersections;
}