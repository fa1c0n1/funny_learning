#include <iostream>
#include "SeqList.hpp"

using namespace std;

class Teacher
{
public:
    char name[64];
    int age;
};

void test1()
{
    Teacher t1, t2, t3, tmp;
    SeqList<Teacher> list(10);

    t1.age = 31;
    t2.age = 32;
    t3.age = 33;

    list.insert(t1, 0);
    list.insert(t2, 0);
    list.insert(t3, 0);

    cout << "insert: "; 
    for (int i = 0; i < list.getLen(); i++) {
        list.get(i, tmp);
        cout << tmp.age << " ";
    }
    cout << endl;

    cout << "clear........" << endl;
    list.clear();
    
    t1.age = 35;
    t2.age = 36;
    t3.age = 37;
    list.insert(t1, 0);
    list.insert(t2, 0);
    list.insert(t3, 0);

    cout << "insert: "; 
    for (int i = 0; i < list.getLen(); i++) {
        list.get(i, tmp);
        cout << tmp.age << " ";
    }
    cout << endl;

    cout << "del: "; 
    while (list.getLen() > 0) {
        list.del(0, tmp); 
        cout << tmp.age << " ";
    }
    cout << endl;
}

void test2()
{
    Teacher t1, t2, t3;
    Teacher *p1, *p2, *p3, *tmp;
    SeqList<Teacher *> list(10);

    t1.age = 31;
    t2.age = 32;
    t3.age = 33;
    p1 = &t1;
    p2 = &t2;
    p3 = &t3;

    list.insert(p1, 0);
    list.insert(p2, 0);
    list.insert(p3, 0);

    cout << "insert: "; 
    for (int i = 0; i < list.getLen(); i++) {
        list.get(i, tmp);
        cout << tmp->age << " ";
    }
    cout << endl;

    

    cout << "del: "; 
    while (list.getLen() > 0) {
        list.del(0, tmp); 
        cout << tmp->age << " ";
    }
    cout << endl;
}

int main()
{
    cout << "----- test1 func ------" << endl;
    test1();
    //cout << "----- test2 func ------" << endl;
    //test2();

    return 0;
}
