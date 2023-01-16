//
// Created by unclec on 18.11.22.
//


#include "minHeap.h"
//#include <algorithm>
using namespace std;
template <class T>
MinHeap<T>::MinHeap(QList<T> init) {
    for (int i = 0; i < init.size(); ++i) {
        push(init[i]);
    }
}

// Elternteil von `A[i]` zurückgeben
// Diese Funktion nicht aufrufen, wenn `i` bereits ein Root-Knoten ist
template <typename T>
int MinHeap<T>::PARENT(int i) {
    return (i - 1) / 2;
}

// linkes Kind von `A[i]` zurückgeben
template <typename T>
int MinHeap<T>::LEFT(int i) {
    return (2*i + 1);
}

// rechtes Kind von `A[i]` zurückgeben
template <typename T>
int MinHeap<T>::RIGHT(int i) {
    return (2*i + 2);
}

// Rekursiver Heapify-Down-Algorithmus.
// Der Knoten am Index `i` und seine zwei direkten Kinder
// verletzt die Heap-Eigenschaft
template <typename T>
void MinHeap<T>::heapify_down(int i) {
    // Holen Sie sich das linke und rechte Kind des Knotens bei Index `i`
    int left = LEFT(i);
    int right = RIGHT(i);

    int smallest = i;

    // Vergleiche `A[i]` mit seinem linken und rechten Kind
    // und finde den kleinsten Wert
//    int tmp = size();
////    cout << tmp << endl;
//    bool tmp1 = left < size();
//    bool tmp2 = A[left] < A[i];
    if (left < size() && A[left] < A[smallest]) {
        smallest = left;
    }
//    int tmp3 = size();
////    cout << tmp3 << endl;
//    bool tmp4 = left < size();
//    bool tmp5 = A[left] < A[i];
    if (right < size() && A[right] < A[smallest]) {
        smallest = right;
    }

    // mit einem Kind mit geringerem Wert tauschen und
    // heapify-down für das Kind aufrufen
    if (smallest != i) {
        swap(A[i], A[smallest]);
        heapify_down(smallest);
    }
}

// Rekursiver Heapify-Up-Algorithmus
template <typename T>
void MinHeap<T>::heapify_up(int i) {
    // prüfen, ob der Knoten am Index `i` und sein Elternteil die Heap-Eigenschaft verletzen
    if (i && A[PARENT(i)] > A[i]) {
        // tausche die beiden aus, wenn die Heap-Eigenschaft verletzt wird
        swap(A[i], A[PARENT(i)]);

        // heapify-up auf dem Elternteil aufrufen
        heapify_up(PARENT(i));
    }
}

// Schlüssel in den Heap einfügen
template <typename T>
void MinHeap<T>::push(T key) {
    // neues Element am Ende des Vectors einfügen
    A.push_back(key);

    // Elementindex abrufen und Heapify-up-Prozedur aufrufen
    int index = size() -1;
    heapify_up(index);
}

template <typename T>
void MinHeap<T>::pop() {
    try {
        // Wenn der Heap keine Elemente hat, löse eine Ausnahme aus
        if (size() == 0) {
            throw std::out_of_range("Vector<T>::at() : "
                                    "index is out of range(Heap underflow)");
        }
        // ersetze die Wurzel des Heaps durch das letzte Element
        // des Vectors
        A[0] = A.back();
        A.pop_back();

        // heapify-down auf dem Root-Knoten aufrufen
        heapify_down(0);
    }
    // Ausnahme abfangen und ausgeben
    catch (const std::out_of_range &oor){
        std::cout << std::endl << oor.what();
    }
}

template <typename T>
T MinHeap<T>::top() {
    try {
        // Wenn der Heap keine Elemente hat, löse eine Ausnahme aus
        if (size() == 0) {
            throw std::out_of_range("Vector<T>::at() : "
                                    "index is out of range(Heap underflow)");
        }

        // Sonst das oberste (erste) Element zurückgeben
        return A.front();
    }
    // Ausnahme abfangen und ausgeben
    catch (const std::out_of_range &oor){
        std::cout << std::endl << oor.what();
    }
    // for compiler, this return statement should never be reached
    return A.front();
}


// Größe des Heaps zurückgeben
template <typename T>
unsigned int MinHeap<T>::size() {
    return A.size();
}