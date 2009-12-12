#ifndef POINT_ARRAY_LIST_H
#define POINT_ARRAY_LIST_H

#include "ADM_Point.h"

#define LIST_CAPACITY 32


class PointArrayList {
private:
    int itemCount;
    p_Point items[LIST_CAPACITY];

public:
    PointArrayList();
    ~PointArrayList();
    void copy(const PointArrayList &src);
    void freeMem();
    void reset();
    bool isEmpty() const;
    int count() const;
    p_Point get(const int index) const;
    int search(const int x) const;
    bool insert(const int index, int x, int y);
    void remove(const int index);
    char *serialize() const;
    void deserialize(char *str);
    void println() const;
    void printAll() const;
};

#endif // POINT_ARRAY_LIST
