#include <stdlib.h>
#include "point.h"

Point newPoint(int x, int y)
{
        Point ret = {.x = x, .y = y};
        return ret;
}

Point relative2(Point point, Point relative)
{
        Point ret = {.x = point.x - relative.x, .y = point.y - relative.y};

        return ret;
}

Point *get_rad(Point center_point, uint8_t radius)
{
        Point *ret;

        

        return ret;
}

int bresenham_circular_size_query(Point center_point, int radius)
{
        int f = 1 - radius;
        int ddF_x = 0;
        int ddF_y = -2 * radius;
        int x = 0;
        int y = radius;
        int ret = 0;

        ret += 4;

        while(x < y) {
                if (f >= 0) {
                        y -= 1;
                        ddF_y += 2;
                        f += ddF_y;
                }
                x += 1;
                ddF_x += 2;
                f += ddF_x + 1;

                ret += 8;
        }

        return ret;
}

Point *bresenham_circular(Point center_point, int radius)
{
        Point *ret;
        int f = 1 - radius;
        int ddF_x = 0;
        int ddF_y = -2 * radius;
        int x = 0;
        int y = radius;
        int idx = 0;

        ret = (Point *) calloc(bresenham_circular_size_query(center_point, radius), sizeof(Point));
        ret[idx] = newPoint(center_point.x, center_point.y + radius);
        ret[idx + 1] = newPoint(center_point.x, center_point.y - radius);
        ret[idx + 2] = newPoint(center_point.x + radius, center_point.y);
        ret[idx + 3] = newPoint(center_point.x - radius, center_point.y);
        idx += 4;

        while(x < y) {
                if (f >= 0) {
                        y -= 1;
                        ddF_y += 2;
                        f += ddF_y;
                }
                x += 1;
                ddF_x += 2;
                f += ddF_x + 1;

                ret[idx] = newPoint(center_point.x + x, center_point.y + y);
                ret[idx + 1] = newPoint(center_point.x - x, center_point.y + y);
                ret[idx + 2] = newPoint(center_point.x + x, center_point.y - y);
                ret[idx + 3] = newPoint(center_point.x - x, center_point.y - y);
                ret[idx + 4] = newPoint(center_point.x + y, center_point.y + x);
                ret[idx + 5] = newPoint(center_point.x - y, center_point.y + x);
                ret[idx + 6] = newPoint(center_point.x + y, center_point.y - x);
                ret[idx + 7] = newPoint(center_point.x - y, center_point.y - x);

                idx += 8;
        }

        return ret;
}

