#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <math.h>

#define POINT_RADIUS 6.0f

typedef struct point {
    int x;
    int y;
    point(int x = 0, int y = 0): x(x), y(y) {}
    point(const point &src) { x = src.x; y = src.y; }
    point(const char *str)
    {
        if (str) sscanf(str, "[%d;%d]", &x, &y);
        else x = y = -1;
    }
    void set(int newx, int newy) { x = newx; y = newy; }
    bool contains(int testx, int testy) const
    {
        return sqrt((testx-x)*(testx-x) + (testy-y)*(testy-y)) < POINT_RADIUS;
    }
    int serialize(char *str)
    {
        return sprintf(str, "[%d;%d]", x, y);
    }
    void print() { printf("[%d;%d]", x, y); }
    void println() { printf("[%d;%d]\n", x, y); }
} Point, *p_Point;

#endif

