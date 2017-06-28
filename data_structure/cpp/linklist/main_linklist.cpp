#include "LinkList.hpp"

class Teacher
{
public:
    char name[64];
    int age;
};

int main()
{
    Teacher t1, t2, t3, tmp;
    LinkList<Teacher> list;
    
    t1.age = 41;
    t2.age = 42;
    t3.age = 43;

    list.insert(t1, 0);
    list.insert(t2, 0);
    list.insert(t3, 0);

    cout << "insert: ";
    for (int i = 0; i < list.len(); i++) {
        list.get(i, tmp); 
        cout << tmp.age << " ";
    }
    cout << endl;

    cout << "clear.........." << endl;
    list.clear();

    t1.age = 45;
    t2.age = 46;
    t3.age = 47;

    list.insert(t1, 0);
    list.insert(t2, 0);
    list.insert(t3, 0);

    cout << "insert: ";
    for (int i = 0; i < list.len(); i++) {
        list.get(i, tmp); 
        cout << tmp.age << " ";
    }
    cout << endl;

    cout << "delete: ";
    while (list.len() > 0) {
        list.del(0, tmp); 
        cout << tmp.age << " ";
    }
    cout << endl;



    return 0;
}
