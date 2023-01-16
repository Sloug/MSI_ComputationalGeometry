//
// Created by unclec on 18.11.22.
//

#ifndef COMPUTATIONALGEOMETRY_QELEMENT_H
#define COMPUTATIONALGEOMETRY_QELEMENT_H

#include <QList>
#include "lines.h"

enum State {vertical, horizontalStart, horizontalEnd, none};

class CoordLineLink {
private:
    int lineIndex;
    double lineCoordinate;
    State state;

public:
    CoordLineLink() {};
    CoordLineLink(int lI, double lC) {
        lineIndex = lI;
        lineCoordinate = lC;
        state = State::none;
    }
    CoordLineLink(int lI, double lC, State s) {
        lineIndex = lI;
        lineCoordinate = lC;
        state = s;
    }
    bool isStart() {
        return state == State::horizontalStart;
    }
    bool isEnd() {
        return state == State::horizontalEnd;
    }
    bool isVertical() {
        return state == State::vertical;
    }
    int getIndex() {
        return lineIndex;
    }
    double getCoordinate() {
        return lineCoordinate;
    }

    static QList<CoordLineLink> linesToQ(Lines lines) {
        QList<CoordLineLink> allElements;
        for (int i = 0; i < lines.getCount(); ++i) {
            if (lines.isVertical(i)) {
                allElements.append(CoordLineLink(i, lines.getStartPointX(i), State::vertical));
            } else {
                allElements.append(CoordLineLink(i, lines.getStartPointX(i), State::horizontalStart));
                allElements.append(CoordLineLink(i, lines.getEndPointX(i), State::horizontalEnd));
            }

        }
        return allElements;
    }
    bool operator >= (const CoordLineLink& cll) {
        return this->lineCoordinate >= cll.lineCoordinate;
    }
    bool operator > (const CoordLineLink& cll) {
        return this->lineCoordinate > cll.lineCoordinate;
    }
    bool operator <= (const CoordLineLink& cll) {
        return this->lineCoordinate <= cll.lineCoordinate;
    }
    bool operator < (const CoordLineLink& cll) {
        return this->lineCoordinate < cll.lineCoordinate;
    }
    bool operator == (const CoordLineLink& cll) {
        return this->lineCoordinate == cll.lineCoordinate;
//        && this->lineIndex == cll.lineIndex;
    }
    bool operator != (const CoordLineLink& cll) {
        return this->lineCoordinate != cll.lineCoordinate;
    }
};

#endif //COMPUTATIONALGEOMETRY_QELEMENT_H

