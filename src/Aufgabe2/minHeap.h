//
// Created by unclec on 18.11.22.
//

#ifndef COMPUTATIONALGEOMETRY_MINHEAP_H
#define COMPUTATIONALGEOMETRY_MINHEAP_H

#include <QList>
#include <iostream>
#include <vector>
//#include <algorithm>


template <typename T>
class MinHeap {
public:
    MinHeap<T>(QList<T> init);
    unsigned int size();
    void push(T key);
    void pop();
    T top();

private:
    std::vector<T> A;
    int PARENT(int i);
    int LEFT(int i);
    int RIGHT(int i);
    void heapify_down(int i);
    void heapify_up(int i);
};

#include "minHeap.tpp"

#endif //COMPUTATIONALGEOMETRY_MINHEAP_H
