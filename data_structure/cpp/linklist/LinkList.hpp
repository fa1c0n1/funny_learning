#include "LinkList.h"
#include <iostream>

using namespace std;

template <typename T>
LinkList<T>::LinkList()
{
    this->header = new LinkListNode<T>;
    this->header->next = NULL;
    this->length = 0;
}

template <typename T>
LinkList<T>::~LinkList()
{
    LinkListNode<T> *curNode = this->header; 
    LinkListNode<T> *tmpNode = NULL;

    for (int i = 0; i < this->length; i++) {
        tmpNode = curNode->next;
        delete curNode;
        curNode = tmpNode;
    }

    this->length = 0;
    this->header = NULL;
}

template <typename T>
void LinkList<T>::clear()
{
    LinkListNode<T> *curNode = this->header; 
    LinkListNode<T> *tmpNode = NULL;

    for (int i = 0; i < this->length; i++) {
        tmpNode = curNode->next;
        delete curNode;
        curNode = tmpNode;
    }

    this->header = new LinkListNode<T>;
    this->header->next = NULL;
    this->length = 0;
}

template <typename T>
int LinkList<T>::len()
{
    return this->length;
}

template <typename T>
int LinkList<T>::insert(T &node, int pos)
{
    LinkListNode<T> *curNode = this->header;
    LinkListNode<T> *newNode = new LinkListNode<T>;
    newNode->tobj = node;
    newNode->next = NULL;

    for (int i = 0; i < pos; i++) {
        curNode = curNode->next; 
    }
    newNode->next = curNode->next;
    curNode->next = newNode;
    this->length++;

    return 0;
}

template <typename T>
int LinkList<T>::get(int pos, T &node)
{
    LinkListNode<T> *curNode = this->header;

    for (int i = 0; i < pos; i++) {
        curNode = curNode->next; 
    }
    node = curNode->next->tobj;

    return 0;
}

template <typename T>
int LinkList<T>::del(int pos, T &node)
{
    LinkListNode<T> *curNode = this->header;
    LinkListNode<T> *delNode = NULL;

    for (int i = 0; i < pos; i++) {
        curNode = curNode->next; 
    }
    delNode = curNode->next;
    curNode->next = delNode->next;
    delNode->next = NULL;
    node = delNode->tobj;

    this->length--;
    delete delNode;

    return 0;
}
