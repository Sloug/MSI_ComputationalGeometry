//
// Created by unclec on 27.11.22.
//

#ifndef TEST_AVL2_AVLTREE_H
#define TEST_AVL2_AVLTREE_H

#pragma once

#include <algorithm>
#include <assert.h>
using namespace std;
template <typename T>
class Node {
public:
    int height;
    T key;
    Node* left;
    Node* right;
    Node* pre;
    Node* next;

    Node() {
        height = -1;
        left = nullptr;
        right = nullptr;
        pre = nullptr;
        next = nullptr;
    }
};

template <typename T>
class AvlTree {
public:
    AvlTree() {
        root = nullptr;
    }
    bool insert(T key) {
//        return insert(key, root, root == nullptr ? nullptr : root->pre, root == nullptr ? nullptr : root->next);
        return insert(key, root, nullptr, nullptr);
    }
    bool remove(T key){
        return remove(key, root);
    }
    void inorder() {
        inorder(root);
    }
    void inorder_links() {
        inorder_links(root);
    }
    void inorder_links_links() {
        inorder_links_links(root);
    }

    Node<T>* findNodeWithSmallestKeyGreaterEqual(T y){
        return findNodeWithSmallestKeyGreaterEqual(root, y);
    }

private:
    Node<T> *root;
//    template <typename T>
    bool insert(T key, Node<T>*& p, Node<T>* pre, Node<T>* next){
        bool inserted;
        if (p == nullptr) {
            p = new Node<T>;
            p->key = key;
            p->height = 0;  // leave has height 0
            if (pre != nullptr){
                p->pre = pre;
                pre->next = p;
            }
            if (next != nullptr) {
                p->next = next;
                next->pre = p;
            }
            inserted = true;
        } else if (key < p->key) {
            inserted = insert(key, p->left, pre, p);
        } else if (key > p->key) {
            inserted = insert(key, p->right, p, next);
        } else {
            inserted = false;  // key exists
        }

        if (inserted) {
            balance(p);
        }

        return inserted;
    }

//    template <typename KeyType, typename ValueType>
    bool remove(T key, Node<T>*& p){
        bool removed = false;
        if (p == nullptr) {
            removed = false;
        } else if (key < p->key) {
            removed = remove(key, p->left);
        } else if (key > p->key) {
            removed = remove(key, p->right);
        } else if (p->left == nullptr || p->right == nullptr) {
            // p needs to be removed and has max one child
            Node<T> *q = p;
            p = p->left != nullptr ? p->left : p->right;
            if (q->pre != nullptr) {
                q->pre->next = q->next;
            }
            if (q->next != nullptr) {
                q->next->pre = q->pre;
            }
            delete(q);
            removed = true;
        } else {
            // p needs to be removed and has two children
            Node<T> min;
            p->right = getRemMin(p->right, min);
            p->key = min.key;
            p->next = min.next;
            if (p->next != nullptr) {
                p->next->pre = p;
            }
            removed = true;
        }

        if (removed) {
            balance(p);
        }

        return removed;
    }

//    template <typename KeyType, typename ValueType>
    Node<T>* getRemMin(Node<T>*& p, Node<T>& min) {
        assert(p != nullptr);
        if (p->left == nullptr) {
            min.key = p->key;
            min.next = p->next;
            min.pre = p->pre;
            Node<T> *q = p;
            p = p->right;
            delete(q);
        } else {
            p->left = getRemMin(p->left, min);
        }

        return p;
    }

//    template <typename KeyType, typename ValueType>
    Node<T>* findNodeWithSmallestKeyGreaterEqual(Node<T>* root, T y){
        Node<T>* p = root;
        Node<T>* q = nullptr;
        while ((p != nullptr) && (p->key != y)) {
            q = p;
            if (y < p->key) {
                p = p->left;
            } else {
                p = p->right;
            }
        }

        if (p != nullptr) {
            return p; // key == y found
        }
        if (root == nullptr) {
            return nullptr; // empty tree
        }
        if (q == root) {
            // tree has only one node
            if (q->key < y) {
                return nullptr; // root not in range
            } else {
                return q; // root in range
            }
        }
        if (y < q->key) {
            return q;
        } else {
            return q->next;
        }
    }

//    template <typename KeyType, typename ValueType>
    void balance(Node<T>*& p) {
        if (p == nullptr) {
            return;
        }

        // adjust height
        p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
        if (getBalance(p) == -2) { // tree is left leaning
            if (getBalance(p->left) <= 0) {
                rotateRight(p); // A1
            } else {
                rotateLeftRight(p); // A2
            }
        } else if(getBalance(p) == +2) { // tree is right leaning
            if (getBalance(p->right) >= 0) {
                rotateLeft(p); // B1
            } else {
                rotateRightLeft(p); // B2
            }
        }
    }

//    template <typename KeyType, typename ValueType>
    int getHeight(const Node<T>* const p){
        if (p == nullptr) {
            return -1; // empty tree
        }

        return p->height;
    }

//    template <typename KeyType, typename ValueType>
    int getBalance(const Node<T>* const p){
        if (p == nullptr) {
            return 0; // empty tree
        }

        return getHeight(p->right) - getHeight(p->left);
    }

//    template <typename KeyType, typename ValueType>
    void rotateRight(Node<T>*& p){
        if (p->left == nullptr) {
            return;
        }

        Node<T> *q = p->left;
        p->left = q->right;
        q->right = p;

        p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
        q->height = std::max( getHeight(q->left), getHeight(q->right)) + 1;
        p = q;
    }

//    template <typename KeyType, typename ValueType>
    void rotateLeft(Node<T>*& p) {
        if (p->right == nullptr) {
            return;
        }

        Node<T> *q = p->right;
        p->right = q->left;
        q->left = p;

        p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
        q->height = std::max( getHeight(q->left), getHeight(q->right)) + 1;
        p = q;
    }

//    template <typename KeyType, typename ValueType>
    void rotateLeftRight(Node<T>*& p) {
        if (p->left == nullptr) {
            return;
        }

        rotateLeft(p->left);
        rotateRight(p);
    }

//    template <typename KeyType, typename ValueType>
    void rotateRightLeft(Node<T>*& p) {
        if (p->right == nullptr) {
            return;
        }

        rotateRight(p->right);
        rotateLeft(p);
    }
    void inorder(Node<T> *t) {
        if (t == NULL)
            return;
        inorder(t->left);
        cout << t->key << " ";
        inorder(t->right);
    }
    void inorder_links(Node<T> *t) {
        Node<T> *tmp = t;
        while (tmp->left != NULL)
            tmp = tmp->left;
        while (tmp->next != NULL) {
            cout << tmp->key << " ";
            tmp = tmp->next;
        }
        cout << tmp->key << " ";
    }

    void inorder_links_links(Node<T> *t) {
        Node<T> *tmp = t;
        while (tmp->pre != NULL)
            tmp = tmp->pre;
        while (tmp->next != NULL) {
            cout << tmp->key << " ";
            tmp = tmp->next;
        }
        cout << tmp->key << " ";
    }

}; // AvlTree

#endif //TEST_AVL2_AVLTREE_H
