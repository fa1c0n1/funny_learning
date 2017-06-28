#include "SeqList.h"
#include <iostream>

using namespace std;

template <typename T>
SeqList<T>::SeqList(int capacity)
{
    pArray = new T[capacity];
    this->capacity = capacity;
    length = 0;
}

template <typename T>
SeqList<T>::~SeqList()
{
    delete [] pArray;
    capacity = 0;
    length = 0;
}

template <typename T>
int SeqList<T>::getLen()
{
    return length;
}

template <typename T>
int SeqList<T>::getCapacity()
{
    return capacity;
}

template <typename T>
int SeqList<T>::insert(T &node, int pos)
{
    if (pos < 0) {
        cout << "Invalid params." << endl;
        return -1;
    }

    if (length >= capacity) {
        cout << "The capacity of seqlist is fully occupied." << endl; 
        return -1;
    }

    if (pos >= capacity) {
        cout << "The param pos is bigger than capacity." << endl; 
        return -1;
    }

    //e.g, length=7, capacity=15, but pos=10
    if (pos > length && pos < capacity) {
        pos = length; 
    }

    for (int i = length; i > pos; i--) {
        pArray[i] = pArray[i-1]; 
    }
    pArray[pos] = node;
    length++;

    return 0;
}

template <typename T>
int SeqList<T>::get(int pos, T &node)
{
    if (pos < 0) {
        cout << "Invalid params." << endl; 
        return -1;
    }

    if (pos >= capacity) {
        cout << "The param pos is bigger than capacity." << endl; 
        return -1;
    }

    node = pArray[pos];

    return 0;    
}

template <typename T>
int SeqList<T>::del(int pos, T &node)
{
    if (pos < 0) {
        cout << "Invalid params." << endl;
        return -1;
    }

    if (pos >= capacity) {
        cout << "The param pos is bigger than capacity." << endl; 
        return -1;
    }

    node = pArray[pos];
    for (int i=pos+1; i < length; i++) {
        pArray[i-1] = pArray[i]; 
    }
    length--;

    return 0;
}
template <typename T>
int SeqList<T>::clear()
{
    this->length = 0;
    return 0;
}
