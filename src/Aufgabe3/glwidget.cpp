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

using namespace std;


GLWidget::GLWidget(RangeTree *rT, QWidget *parent) : QOpenGLWidget(parent)
{
    rangeTree = rT;
    pointsSize = 10;
    generatePoints();
    rectangle = false;
}

GLWidget::~GLWidget()
{
}

typedef std::pair<QPointF, QPointF> lineSeg;
void GLWidget::paintGL()
{
    cout << "New Painting" << endl;
    // clear
    glClear(GL_COLOR_BUFFER_BIT);

    // Punkte

    glPointSize(7.0);
    glBegin(GL_POINTS);
    for (int i=0; i<points.getCount(); i++) {
        if(rangeTree->rangeContains(QPointF(points.getPointX(i),points.getPointY(i))))
            glColor3f(0.0,0.0,1.0);
        else
            glColor3f(1.0,0.0,0.0);
        glVertex2f(points.getPointX(i),points.getPointY(i));
    }

    glEnd();

    //print partitions
    glColor3f(0.0,1.0,1.0);
    glBegin(GL_LINES);

    std::vector<lineSeg> partitions = drawPartitions();
    for (int i = 0; i < partitions.size(); ++i) {
        glVertex2f(partitions[i].first.x(), partitions[i].first.y());
        glVertex2f(partitions[i].second.x(), partitions[i].second.y());
    }
//    int level = 0;
//    Node *k = rangeTree->root;
//    while (k != nullptr) {
//        level++;
//    }

    glEnd();

    if (rectangle) {
        glColor3f(0.0,1.0,0.0);
        glBegin(GL_LINES);

        glVertex2f(firstRectangularPointBuffer.x(), firstRectangularPointBuffer.y());
        glVertex2f(secondRectangularPointBuffer.x(), secondRectangularPointBuffer.y());

        glVertex2f(secondRectangularPointBuffer.x(), secondRectangularPointBuffer.y());
        glVertex2f(thirdRectangularPointBuffer.x(), thirdRectangularPointBuffer.y());

        glVertex2f(thirdRectangularPointBuffer.x(), thirdRectangularPointBuffer.y());
        glVertex2f(fourthRectangularPointBuffer.x(), fourthRectangularPointBuffer.y());

        glVertex2f(fourthRectangularPointBuffer.x(), fourthRectangularPointBuffer.y());
        glVertex2f(firstRectangularPointBuffer.x(), firstRectangularPointBuffer.y());

        glEnd();
    }
}

typedef std::pair<QPointF, QPointF> lineSeg;
std::vector<lineSeg> GLWidget::drawPartitions() const {
    std::vector<lineSeg> partitions;
    drawPartitionsR(-2, 2, -2, 2, true, rangeTree->root, partitions);
    return partitions;
}

void GLWidget::drawPartitionsR(const double minX,const  double maxX, const double minY, const double maxY, const bool vertical, Node *node, std::vector<lineSeg> &partitions) const {
    if (node == nullptr) {
        return;
    }

    if (vertical) { // set y
        partitions.emplace_back(lineSeg({minX, node->value.y()}, {maxX, node->value.y()}));
        drawPartitionsR(minX, maxX, minY, node->value.y(), false, node->left, partitions);
        drawPartitionsR(minX, maxX, node->value.y(), maxY, false, node->right, partitions);
    } else { // set x
        partitions.emplace_back(lineSeg({node->value.x(), minY}, {node->value.x(), maxY}));
        drawPartitionsR(minX, node->value.x(), minY, maxY, true, node->left, partitions);
        drawPartitionsR(node->value.x(), maxX, minY, maxY, true, node->right, partitions);
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
        rectangle = false;
        cout << "Click" << endl;
        firstRectangularPointBuffer = QPointF(posF.x(), posF.y()); //ol
	}
    update(); 
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF posF = transformPosition(event->pos());

    cout << "Released" << endl;
    secondRectangularPointBuffer = QPointF(firstRectangularPointBuffer.x(), posF.y()); //ul
    thirdRectangularPointBuffer = QPointF(posF.x(), posF.y()); //ur
    fourthRectangularPointBuffer = QPointF(posF.x(), firstRectangularPointBuffer.y()); //or
    rectangle = true;
    rangeTree->rangeSearch(firstRectangularPointBuffer, thirdRectangularPointBuffer);
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
    X = points.sortedByX();
    Y = points.sortedByY();
    rangeTree->constructBalanced2dTree(X, Y);
}


