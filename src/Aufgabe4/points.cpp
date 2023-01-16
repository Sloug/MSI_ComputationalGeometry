#include "points.h"


Points::Points()
{
}

void Points::addPoint(float x,float y)
{
    pointlist.append(QPointF(x,y));
}

void Points::clearPoints() {
    pointlist.clear();
}

float Points::getPointX(int i)
{
    return pointlist[i].x();
}

float Points::getPointY(int i)
{
    return pointlist[i].y();
}

void Points::setPointX(int i, float x)
{
    return pointlist[i].setX(x);
}

void Points::setPointY(int i, float y)
{
    return pointlist[i].setY(y);
}

int Points::getCount()
{
    return pointlist.size();
}

int Points::getClosestPoint(float x, float y)
{
    int result = 0;
    QPointF testpnt = QPointF(x,y);
    QPointF distpnt = pointlist.first()-testpnt;
    float distance = distpnt.x()*distpnt.x() + distpnt.y() * distpnt.y();
    float mindist = distance;
    for (int i = 0; i < pointlist.size(); i++) {
        QPointF& pnt = pointlist[i];
        distpnt = pnt-testpnt;
        distance = distpnt.x()*distpnt.x() + distpnt.y() * distpnt.y();
        if (distance < mindist) {
            mindist = distance;
            result = i;
        }
    }
    return result;
}

void Points::moveClosestPoint(float x, float y, float tox, float toy)
{
    int pntnum = getClosestPoint(x,y);
    pointlist[pntnum].setX(tox);
    pointlist[pntnum].setY(toy);
}

int   Points::getMinX()
{
    float minX = std::numeric_limits<float>::max();
    int index = 0;
    for (int i = 0; i < getCount(); ++i) {
        if (pointlist[i].x() < minX) {
            minX = pointlist[i].x();
            index = i;
        }
    }
    return index;
}
int   Points::getMinY()
{
    float minY = std::numeric_limits<float>::max();
    int index = 0;
    for (int i = 0; i < getCount(); ++i) {
        if (pointlist[i].y() < minY) {
            minY = pointlist[i].y();
            index = i;
        }
    }
    return index;
}

int   Points::getMinYX()
{
    float minY = std::numeric_limits<float>::max();
    float minYX = std::numeric_limits<float>::max();
    int index = 0;
    for (int i = 0; i < getCount(); ++i) {
        if (pointlist[i].y() < minY) {
            minY = pointlist[i].y();
            minYX = pointlist[i].x();
            index = i;
        } else if (pointlist[i].y() == minY && pointlist[i].x() < minYX) {
            minY = pointlist[i].y();
            minYX = pointlist[i].x();
            index = i;
        }
    }
    return index;
}

int   Points::getMaxX()
{
    float maxX = std::numeric_limits<float>::lowest();
    int index = 0;
    for (int i = 0; i < getCount(); ++i) {
        if (pointlist[i].x() > maxX) {
            maxX = pointlist[i].x();
            index = i;
        }
    }
    return index;
}
int   Points::getMaxY()
{
    float maxY = std::numeric_limits<float>::lowest();
    int index = 0;
    for (int i = 0; i < getCount(); ++i) {
        if (pointlist[i].y() > maxY) {
            maxY = pointlist[i].y();
            index = i;
        }
    }
    return index;
}

static bool compare(const QPointF& first, const QPointF& second)
{
    if (first.x() < second.x())
    {
        return true;
    }
    else if (first.x() > second.x())
    {
        return false;
    }
    else
    {
        if (first.y() < second.y())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}

void Points::sortByX() {
    std::sort(pointlist.begin(), pointlist.end(), compare);
}

QList<QPointF> Points::getPoints() {
    return pointlist;
}

bool Points::contains(QPointF p) {
    bool output = false;
    for (int i = 0; i < getCount(); ++i) {
        if (pointlist[i] == p) {
            output = true;
            break;
        }
    }
    return output;
}

double Points::getAngle(QPointF p0, QPointF p1, QPointF center) {
    QPointF p0c = QPointF(p0.x()-center.x(), p0.y()-center.y());
    QPointF p1c = QPointF(p1.x()-center.x(), p1.y()-center.y());
    double angle = atan2(p0c.y(), p0c.x()) - atan2(p1c.y(), p1c.x());
    return angle;
}

typedef std::pair<double, int> anlgeIndex;
QList<QPointF> Points::clockwise(QPointF center) {
    QList<anlgeIndex> output;
    QPointF centerHelper = QPointF(center.x(), center.y() - 1);

    for (int i = 0; i < getCount(); ++i) {
        double angle = getAngle(centerHelper, pointlist[i], center);
        output.append(anlgeIndex(angle, i));
    }
    std::sort(output.begin(), output.end());
    QList<QPointF> out;
    for (int i = 0; i < output.size(); ++i) {
        out.append(pointlist[output[i].second]);
    }
    return out;
}