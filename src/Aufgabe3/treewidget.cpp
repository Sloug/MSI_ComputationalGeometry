//
// Created by unclec on 05.12.22.
//

#include "treewidget.h"
#include <iostream>

using namespace std;

TreeWidget::TreeWidget(RangeTree *rT, QWidget *parent) : QOpenGLWidget(parent)
{
    rangeTree = rT;
}

TreeWidget::~TreeWidget()
{
}

bool TreeWidget::printLevel(Node *root, int level) {
    if(root == nullptr) {
        treeNodes.append(false);
        rangeNodes.append(false);
        return false;
    }
    if(level == 0) {
        treeNodes.append(true);
        if (rangeTree->rangeContains(root->value))
            rangeNodes.append(true);
        else
            rangeNodes.append(false);
        return true;
    }

    bool left = printLevel(root->left, level - 1);
    bool right = printLevel(root->right, level - 1);
    return left || right;
}

void TreeWidget::paintGL()
{
    cout << "Tree Painting" << endl;
    qDebug() << rangeNodes.contains(true);
    // clear
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(7.0);
    glBegin(GL_POINTS);

    treeNodes.clear();
    rangeNodes.clear();
    int level = 0;
    while (printLevel(rangeTree->root, level)) {
        level++;
    }
    level = 0;
    int maximumInLevel = 1;
    int inLevel = 0;
    for (int i = 0; i < treeNodes.size(); ++i) {
        if (maximumInLevel == inLevel) {
            level++;
            maximumInLevel = std::pow(2, level);
            inLevel = 0; // Todo: 1 oder 0
        }
        if(treeNodes[i]) {
//            qDebug() << "i: " << i;
            if (rangeNodes[i]) {
//                qDebug() << "Blue";
                glColor3f(0.0, 0.0, 1.0);
            } else
                glColor3f(1.0, 0.0, 0.0);
//            qDebug() << "inLevel: " << inLevel;
//            qDebug() << "maximumInLevel: " << maximumInLevel;
//            qDebug() << "Knot at: X: " << (-1.0 + ((double) inLevel + 1.0) * (2.0 / ((double) maximumInLevel + 1))) << " Y: " << (0.9 - (0.2 * level));
            glVertex2f((-1.0 + ((double) inLevel + 1.0) * (2.0 / ((double) maximumInLevel + 1))),(0.9 - (0.2 * level)));
//            qDebug() << "level: " << level;
        }
        inLevel++;

    }
    glEnd();

//    glPointSize(7.0);
//    glBegin(GL_POINTS);
//    if (rangeTree->root != nullptr) {
//        if (rangeTree->rangeContains(rangeTree->root->value)) {
////            qDebug() << "Range contains root";
//            glColor3f(0.0, 0.0, 1.0);
//        } else {
////            qDebug() << "Range not contains root";
//            glColor3f(1.0, 0.0, 0.0);
//        }
////        (-1 + (inLevel + 1) * (2 / (maximumInLevel + 1)));
//        glVertex2f((-1 + (inLevel + 1) * (2 / (maximumInLevel + 1))),(0.9 - (0.2 * level)));
//    }

    // Punkte

//    glPointSize(7.0);
//    glBegin(GL_POINTS);
//    glColor3f(1.0,0.0,0.0);
//    glVertex2f(0,0.9);
//
//    glVertex2f(-0.4,0.7);
//    glVertex2f(0.4,0.7);
//
//    glVertex2f(-0.6,0.5);
//    glVertex2f(-0.2,0.5);
//    glVertex2f(0.2,0.5);
//    glVertex2f(0.6,0.5);
//
//    glVertex2f(-0.7,0.3);
//    glVertex2f(-0.5,0.3);
//    glVertex2f(-0.3,0.3);
//
//
//    glEnd();
//
//    glColor3f(0.0,1.0,0.0);
//    glBegin(GL_LINES);
//    glVertex2f(0,0.9);
//    glVertex2f(-0.4,0.7);
//
//    glVertex2f(0,0.9);
//    glVertex2f(0.4,0.7);
//
//    glVertex2f(-0.4,0.7);
//    glVertex2f(-0.6,0.5);
//    glVertex2f(-0.4,0.7);
//    glVertex2f(-0.2,0.5);
//    glVertex2f(0.4,0.7);
//    glVertex2f(0.2,0.5);
//    glVertex2f(0.4,0.7);
//    glVertex2f(0.6,0.5);
//
//    glVertex2f(-0.6,0.5);
//    glVertex2f(-0.7,0.3);
//    glVertex2f(-0.6,0.5);
//    glVertex2f(-0.5,0.3);
//    glVertex2f(-0.2,0.5);
//    glVertex2f(-0.3,0.3);
//

}
