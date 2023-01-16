#ifndef POINTS_H
#define POINTS_H
#include <QList>
#include <QPointF>

class Points
{
public:
    Points();
    void  addPoint (float x, float y);
    void  clearPoints();
    void  sortByX();
    float getPointX(int   i);
    float getPointY(int   i);
    void  setPointX(int   i, float x);
    void  setPointY(int   i, float y);
    int   getCount ();
    int   getClosestPoint (float x, float y);
    void  moveClosestPoint(float x, float y, float tox, float toy);
    int   getMinX  ();
    int   getMinY  ();
    int   getMinYX ();
    int   getMaxX  ();
    int   getMaxY  ();
    QList<QPointF> getPoints();
//    QList<QPointF> clockwise();
    QList<QPointF> clockwise(QPointF center);
    bool contains(QPointF p);
private:
    QList<QPointF> pointlist;
    double getAngle(QPointF p0, QPointF p1, QPointF center);
};

#endif // POINTS_H
