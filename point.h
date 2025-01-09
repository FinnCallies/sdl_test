#include <stdint.h>

#ifndef POINT_H
#define POINT_H

typedef struct {
        int x;
        int y;
} Point;

Point newPoint(int x, int y);
Point relative2(Point point, Point relative);
Point *get_rad(Point center_point, uint8_t radius);
Point *bresenham_circular(Point center_point, int radius);

#endif /* POINT_H */
