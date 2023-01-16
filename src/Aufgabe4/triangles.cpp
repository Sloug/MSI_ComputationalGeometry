//
// Created by unclec on 04.01.23.
//

#include "triangles.h"

void Triangles::DrawCircle(float cx, float cy, float r)
{
    float rx = r;
    float ry = r;
    int num_segments = 100;
    float theta = 2 * 3.1415926 / float(num_segments);
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = 1;//we start at angle = 0
    float y = 0;

    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
    {
        //apply radius and offset
        glVertex2f(x * rx + cx, y * ry + cy);//output vertex

        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}

void Triangles::DrawCircle(int i) {
    Circle c = triangles[i].toCircle();
    DrawCircle(c.x, c.y, c.radius);
}

Triangle::Triangle(QPointF a, QPointF b, QPointF c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

QPointF Triangle::getA() {
    return a;
}

QPointF Triangle::getB() {
    return b;
}

QPointF Triangle::getC() {
    return c;
}

bool Triangle::hasVertex(QPointF p) {
    return (this->a == p) || (this->b == p) || (this->c == p);
}

bool Triangle::isAdjacent(Triangle target) {
    return (this->hasVertex(target.a)) || (this->hasVertex(target.b)) || (this->hasVertex(target.c));
}

bool Triangle::hasAdjacentEdge(Triangle target) {
    bool a = this->hasVertex(target.a);
    bool b = this->hasVertex(target.b);
    bool c = this->hasVertex(target.c);
    return (a && b) || (a && c) || (b && c);
}

// Returns distinct target point at index 0 and distinct point of this at index 1
QList<QPointF> Triangle::getDistinctPoints(Triangle target) {
    QList<QPointF> output;
    if (!hasAdjacentEdge(target))
        throw std::invalid_argument("Triangles do not have adjacent edges!");
    if(!hasVertex(target.a)) {
        output.append(target.a);
        if(!target.hasVertex(this->a)) {
            output.append(this->a);
        }
        if(!target.hasVertex(this->b)) {
            output.append(this->b);
        }
        if(!target.hasVertex(this->c)) {
            output.append(this->c);
        }
    }
    if(!hasVertex(target.b)) {
        output.append(target.b);
        if (!target.hasVertex(this->a)) {
            output.append(this->a);
        }
        if (!target.hasVertex(this->b)) {
            output.append(this->b);
        }
        if (!target.hasVertex(this->c)) {
            output.append(this->c);
        }
    }
    if(!hasVertex(target.c)) {
        output.append(target.c);
        if(!target.hasVertex(this->a)) {
            output.append(this->a);
        }
        if(!target.hasVertex(this->b)) {
            output.append(this->b);
        }
        if(!target.hasVertex(this->c)) {
            output.append(this->c);
        }
    }
    if(output.size() != 2)
        throw std::invalid_argument("Output size is not correct!");
    return output;
}
// Returns distinct point in target
QPointF Triangle::getDistinctPoint(Triangle target) {


    if (!hasAdjacentEdge(target))
        throw std::invalid_argument("Triangles do not have adjacent edges!");
    if(!hasVertex(target.a))
        return target.a;
    else if(!hasVertex(target.b))
        return target.b;
    else if(!hasVertex(target.c))
        return target.c;
    else
        throw std::runtime_error("Illegal state");
}

Circle Triangle::toCircle() {
    Circle output;
    double d = 2 * (a.x() * (b.y() - c.y()) + b.x() * (c.y() - a.y()) + c.x() * (a.y() - b.y()));
    output.x = 1/d * ((pow(a.x(),2) + pow(a.y(),2)) * (b.y() - c.y()) + (pow(b.x(),2) + pow(b.y(),2)) * (c.y() - a.y()) + (pow(c.x(),2) + pow(c.y(),2)) * (a.y() - b.y()));
    output.y = 1/d * ((pow(a.x(),2) + pow(a.y(),2)) * (c.x() - b.x()) + (pow(b.x(),2) + pow(b.y(),2)) * (a.x() - c.x()) + (pow(c.x(),2) + pow(c.y(),2)) * (b.x() - a.x()));
    output.radius = sqrt(pow((a.x() - output.x),2) + pow((a.y() - output.y),2));
    return output;
}

void Triangles::addTriangle(QPointF a, QPointF b, QPointF c) {
    triangles.append(Triangle(a, b, c));
}

int Triangles::length() {
    return triangles.size();
}

QList<QPointF> Triangles::getPoints(int i) {
    QList<QPointF> output;
    output.append(triangles[i].getA());
    output.append(triangles[i].getB());
    output.append(triangles[i].getC());
    return output;
}

bool Triangles::isAdjacentTo(int source, int target) {
    return triangles[source].isAdjacent(triangles[target]);
}

bool Triangles::containsPointInCircle(int i, QPointF p) {
//    Circle circle = triangles[i].toCircle();
//    DrawCircle(circle.x, circle.y, circle.radius);
    return triangles[i].toCircle().circleContainsPoint(p);
}

void Triangles::remove(QList<int> indexes) {
    std::sort(indexes.begin(), indexes.end());
    for (int i = indexes.size() - 1; i >= 0; --i) {
        Triangle removed = triangles[indexes[i]];
        triangles.removeAt(indexes[i]);
    }
}

Points Triangles::distinctPoints(QList<int> indexes) {
    Points output;



    for (int i = 0; i < indexes.size(); ++i) {
        if (!output.contains(triangles[indexes[i]].getA()))
            output.addPoint(triangles[indexes[i]].getA().x(), triangles[indexes[i]].getA().y());
        if (!output.contains(triangles[indexes[i]].getB()))
            output.addPoint(triangles[indexes[i]].getB().x(), triangles[indexes[i]].getB().y());
        if (!output.contains(triangles[indexes[i]].getC()))
            output.addPoint(triangles[indexes[i]].getC().x(), triangles[indexes[i]].getC().y());
    }
    return output;
}

bool Triangles::hasAdjacentEdgeTo(int source, int target) {
    return triangles[source].hasAdjacentEdge(triangles[target]);
}

void Triangles::restoreDelauneyByFLipping() {
    for (int i = 0; i < length(); ++i) {
        for (int j = 0; j < length(); ++j) {
            if(i != j && hasAdjacentEdgeTo(i, j)) {
                // p ist der punkt in triangles[j], der nicht in triangles[i] vorkommt, die anderen beiden kommen vor
                QPointF p = triangles[i].getDistinctPoint(triangles[j]);
                if(containsPointInCircle(i, p)) {
                    // flip edges
                    QList<QPointF> ps = triangles[i].getDistinctPoints(triangles[j]);
                    QPointF firstAdjacentVertex;
                    QPointF secondAdjacentVertex;

                    if(triangles[j].getA() == p) {
                        firstAdjacentVertex = triangles[j].getB();
                        secondAdjacentVertex = triangles[j].getC();
                    } else if(triangles[j].getB() == p) {
                        firstAdjacentVertex = triangles[j].getA();
                        secondAdjacentVertex = triangles[j].getC();
                    } else if(triangles[j].getC() == p) {
                        firstAdjacentVertex = triangles[j].getA();
                        secondAdjacentVertex = triangles[j].getB();
                    } else
                        throw std::invalid_argument("Has no adjacent vertex");
                    triangles[i] = Triangle(ps[0], ps[1], secondAdjacentVertex);
                    triangles[j] = Triangle(ps[0], ps[1], firstAdjacentVertex);
                }
            }
        }
    }
}

void Triangles::mergeTriangles(Triangles tr) {
    triangles.append(tr.triangles);
}

typedef std::pair<QPointF, QPointF> lineSeg;
QList<lineSeg> Triangles::printingPoints() {
    QList<lineSeg> output;
    for (int i = 0; i < length(); ++i) {
        lineSeg a = lineSeg(triangles[i].getA(), triangles[i].getB());
        lineSeg b = lineSeg(triangles[i].getB(), triangles[i].getC());
        lineSeg c = lineSeg(triangles[i].getA(), triangles[i].getC());
        if(!output.contains(a))
            output.append(a);
        if(!output.contains(b))
            output.append(b);
        if(!output.contains(c))
            output.append(c);;
    }
    return output;
}
