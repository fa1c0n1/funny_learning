#pragma once

template <typename T>
class SeqList
{
private:    
    int length;
    int capacity;
    T *pArray;

public:
    SeqList(int capacity);
    ~SeqList();

   int getLen();
   int getCapacity();
   int insert(T &node, int pos); 
   int get(int pos, T &node);
   int del(int pos, T &node);
   int clear();
};
