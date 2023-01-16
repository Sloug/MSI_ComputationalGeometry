//
// Created by unclec on 22.11.22.
//

#include "AvlTree.h"

template <typename T>
int AvlTree<T>::height(AvlNode<T> *t) {
    int h = 0;
    if (t != NULL) {
        int l_height = height(t->l);
        int r_height = height(t->r);
        int max_height = max(l_height, r_height);
        h = max_height + 1;
    }
    return h;
}
template <typename T>
AvlNode<T> *AvlTree<T>::smallestElementGTKey(AvlNode<T> *y) {
    if (r == NULL)
        return NULL;
    if (r->d > y->d) { // key smaller than root -> check if there is an element which is also bigger than key, but less than root
        AvlNode<T> *tmp = r;
        while ((tmp->l != NULL) && (tmp->l->d > y->d))
            tmp = tmp->l;
        return tmp;
    } else { // key bigger or equal to root -> bigger element needed
        AvlNode<T> *tmp = r;
        while ((tmp->r != NULL) && (tmp->d <= y->d))
            tmp = tmp->r;
        while ((tmp->l != NULL) && (tmp->l->d > y->d))
            tmp = tmp->l;
        return tmp->d != y->d ? tmp : NULL;
    }
}
template <typename T>
AvlNode<T> *AvlTree<T>::biggestElementLTKey(AvlNode<T> *y) {
    if (r == NULL)
        return NULL;
    if (r->d < y->d) { // key is bigger than root -> check if there is an element which is also smaller than key, but bigger than root
        AvlNode<T> *tmp = r;
        while ((tmp->r != NULL) && (tmp->r->d < y->d))
            tmp = tmp->r;
        return tmp;
    } else { // key smaller or equal to root -> smaller element needed
        AvlNode<T> *tmp = r;
        while ((tmp->l != NULL) && (tmp->l->d >= y->d))
            tmp = tmp->l;
        while ((tmp->r != NULL) && (tmp->r->d < y->d))
            tmp = tmp->r;
        return tmp->d != y->d ? tmp : NULL;
    }
}
template <typename T>
AvlNode<T> *AvlTree<T>::smallestElementGEKey(T y) {
    AvlNode<T> *p = r;
    AvlNode<T> *q = NULL;
    while ((p != NULL) && (p->d != y)) {
        q = p;
        if (y < p->d)
            p = p->l;
        else p = p->r;
    }
    if (p != NULL)
        return p; // key==y found
    if (r == NULL)
        return NULL; // empty tree
    if (q == r) { // tree has only one knot
//        if (r->d <= y)
        if (r->d < y)
            return NULL;
        else
            return r;
    }

    if (y < q->d)
        return q;
    else
        return q->successor;
}
template <typename T>
void AvlTree<T>::update_links_insert(AvlNode<T> *&element) {
    AvlNode<T> *succ = smallestElementGTKey(element);
    AvlNode<T> *pred;
    if (succ != NULL)
        pred = succ->predessor;
    else
        pred = biggestElementLTKey(element);
    element->successor = succ;
    element->predessor = pred;
    if (pred != NULL)
        pred->successor = element;
    if (succ != NULL)
        succ->predessor = element;
}
template <typename T>
void AvlTree<T>::update_links_remove(AvlNode<T> *&element) {
    AvlNode<T> *tmp = element;
    while(tmp->successor != NULL) {
        if ((element->d == tmp->successor->d)) {
            element = tmp->successor;
        }
        tmp = tmp->successor;
    }
    AvlNode<T> *pred = element->predessor;
    AvlNode<T> *succ = element->successor;
    if (pred != NULL)
        pred->successor = succ;
    if (succ != NULL)
        succ->predessor = pred;
}
template <typename T>
int AvlTree<T>::difference(AvlNode<T> *t) {
    int l_height = height(t->l);
    int r_height = height(t->r);
    int b_factor = l_height - r_height;
    return b_factor;
}
template <typename T>
void AvlTree<T>::rr_rotat(AvlNode<T>* &parent) {
    AvlNode<T> *t;
    t = parent->r;
    parent->r = t->l;
    t->l = parent;
//    cout<<"Right-Right Rotation" << endl;
    parent = t;
}
template <typename T>
void AvlTree<T>::ll_rotat(AvlNode<T>* &parent) {
    AvlNode<T> *t;
    t = parent->l;
    parent->l = t->r;
    t->r = parent;
//    cout<<"Left-Left Rotation" << endl;
    parent = t;
}
template <typename T>
void AvlTree<T>::lr_rotat(AvlNode<T>* &parent) {
    AvlNode<T> *t;
    rr_rotat(parent->l);
//    cout<<"Left-Right Rotation" << endl;
    ll_rotat(parent);
}
template <typename T>
void AvlTree<T>::rl_rotat(AvlNode<T>* &parent) {
    AvlNode<T> *t;
    ll_rotat(parent->r);
//    cout<<"Right-Left Rotation" << endl;
    rr_rotat(parent);
}
template <typename T>
void AvlTree<T>::balance(AvlNode<T>* &t) {
    int bal_factor = difference(t);
    if (bal_factor > 1) {
        if (difference(t->l) > 0)
            ll_rotat(t);
        else
            lr_rotat(t);
    } else if (bal_factor < -1) {
        if (difference(t->r) > 0)
            rl_rotat(t);
        else
            rr_rotat(t);
    }
//    return t;
}
template <typename T>
void AvlTree<T>::insert(T v) {
    insert(r, v);
}
template <typename T>
void AvlTree<T>::insert(AvlNode<T>* &r, T v) {
    if (r == NULL) {
        r = new AvlNode<T>;
        r->d = v;
        r->l = NULL;
        r->r = NULL;
        update_links_insert(r);
    } else if (v< r->d) {
        insert(r->l, v);
        balance(r);
    } else if (v >= r->d) {
        insert(r->r, v);
        balance(r);
    };
}
template <typename T>
AvlNode<T> *AvlTree<T>::remove(T v) {
    AvlNode<T> *tmp = remove(r, v, true);
//    if (tmp != NULL)
//        update_links_remove(tmp); // Todo: sollte nie NULL sein bei aufruf
    return tmp;
}
template <typename T>
AvlNode<T> *AvlTree<T>::remove(AvlNode<T>* &r, T v, bool notMin) {
    AvlNode<T> *tmp;
    if (r == NULL)
        return NULL;
        // Searching for element
    else if (r->d > v)
        tmp = remove(r->l, v, notMin);
    else if (r->d < v) {
        tmp = remove(r->r, v, notMin);
    }
    else {
        tmp = r; // q=p
        bool leftChildNull = r->l == NULL;
        if (r->r != NULL) {

            AvlNode<T> *tmp2 = r->r;
            while (tmp2 ->l != NULL)
                tmp2 = tmp2 ->l; // tmp2 = min
            r -> d = tmp2 -> d;
            remove(r->r,tmp2->d,false);
            if (notMin) {
                if (leftChildNull) {
                    // linkes Kind ist NULL
                    if (tmp->predessor != NULL)
                        tmp->predessor->successor = tmp->successor;
                    if (tmp->successor != NULL)
                        tmp->successor->predessor = tmp->predessor;
                } else {
                    r->successor = tmp2->successor;
                    if (r->successor != NULL)
                        r->successor->predessor = r;
                }
            }
        } else {
            // rechtes Kind ist NULL
            r = r->l;
            if (notMin) {
                if (tmp->predessor != NULL)
                    tmp->predessor->successor = tmp->successor;
                if (tmp->successor != NULL)
                    tmp->successor->predessor = tmp->predessor;
            }
        }
    }
    if (r != NULL)
        balance(r);
    return tmp;
}
template <typename T>
AvlNode<T> *AvlTree<T>::search(T v) {
    return search(r, v);
}
template <typename T>
AvlNode<T> *AvlTree<T>::search(AvlNode<T>* &r, T v) {
    if (r == NULL)
        return NULL;
        // Searching for element
    else if (r->d > v)
        return search(r->l, v);
    else if (r->d < v)
        return search(r->r, v);
    else
        return r;
}
template <typename T>
void AvlTree<T>::show(T l) {
    show(r, l);
}
template <typename T>
void AvlTree<T>::show(AvlNode<T> *p, T l) {
    int i;
    if (p != NULL) {
        show(p->r, l+ 1);
        cout<<" ";
        if (p == r)
            cout << "Root -> ";
        for (i = 0; i < l&& p != r; i++)
            cout << " ";
        cout << p->d;
        show(p->l, l + 1);
    }
}
template <typename T>
void AvlTree<T>::inorder_links() {
    inorder_links(r);
}
template <typename T>
void AvlTree<T>::inorder_links(AvlNode<T> *t) {
    AvlNode<T> *tmp = t;
    while (tmp->l != NULL)
        tmp = tmp->l;
    while (tmp->successor != NULL) {
        cout << tmp->d << " ";
        tmp = tmp->successor;
    }
    cout << tmp->d << " ";
}
template <typename T>
void AvlTree<T>::inorder() {
    inorder(r);
}
template <typename T>
void AvlTree<T>::inorder(AvlNode<T> *t) {
    if (t == NULL)
        return;
    inorder(t->l);
    cout << t->d << " ";
    inorder(t->r);
}
template <typename T>
void AvlTree<T>::preorder(AvlNode<T> *t) {
    if (t == NULL)
        return;
    cout << t->d << " ";
    preorder(t->l);
    preorder(t->r);
}
template <typename T>
void AvlTree<T>::postorder(AvlNode<T> *t) {
    if (t == NULL)
        return;
    postorder(t ->l);
    postorder(t ->r);
    cout << t->d << " ";
}