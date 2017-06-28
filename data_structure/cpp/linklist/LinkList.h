#pragma once

template <typename T>
struct LinkListNode
{
    T tobj;
    LinkListNode *next;
};

template <typename T>
class LinkList
{
private:
    int length;
    LinkListNode<T> *header;

public:
    LinkList();
    ~LinkList();

   void clear(); 
   int len();
   int insert(T &node, int pos);
   int get(int pos, T &node); 
   int del(int pos, T &node);
};
