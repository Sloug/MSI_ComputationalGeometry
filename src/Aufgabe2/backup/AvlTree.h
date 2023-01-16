//
// Created by unclec on 22.11.22.
//

#ifndef TEST_AVL_AVLTREE_H
#define TEST_AVL_AVLTREE_H
#include<iostream>
#include<algorithm>
//#include<sstream>
#include<cstdio>
#include "coordLineLink.h"

using namespace std;
template <typename T>
class AvlNode {
public:
    T d;
    AvlNode *l;
    AvlNode *r;
    AvlNode *predessor;
    AvlNode *successor;
};

template <typename T>
class AvlTree {
private:
    AvlNode<T> *r;
    int height(AvlNode<T> *);
    AvlNode<T> *smallestElementGTKey(AvlNode<T> *);
    AvlNode<T> *biggestElementLTKey(AvlNode<T> *);
    AvlNode<T> *smallestElementGEKey(AvlNode<T> *);
    void update_links_insert(AvlNode<T> *&);
    void update_links_remove(AvlNode<T> *&);
    int difference(AvlNode<T> *);
    void rr_rotat(AvlNode<T> *&);
    void ll_rotat(AvlNode<T> *&);
    void lr_rotat(AvlNode<T> *&);
    void rl_rotat(AvlNode<T> *&);
    void balance(AvlNode<T> *&);
    void insert(AvlNode<T>*&, T);
    AvlNode<T> *search(AvlNode<T>*&, T);
    AvlNode<T> *remove(AvlNode<T>*&, T, bool);
    void show(AvlNode<T>*, T);
    void inorder(AvlNode<T> *);
    void inorder_links(AvlNode<T> *);
    void preorder(AvlNode<T> *);
    void postorder(AvlNode<T>*);
public:
    AvlNode<T> *smallestElementGEKey(T);
    void insert(T);
    AvlNode<T> *search(T);
    AvlNode<T> *remove(T);
    void show(T);
    void inorder();
    void inorder_links();
    void preorder();
    void postorder();
    AvlTree() {
        r = NULL;
    }
};
#include "AvlTree.tpp"


#endif //TEST_AVL_AVLTREE_H
