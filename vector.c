#include "vector.h"

void normalize(Vector *v)
{
        if (v->x < 0)
                v->x = v->x * -1;
        if (v->y < 0)
                v->y = v->y * -1;
}

Vector pts2vec(Point a, Point b)
{
        Vector ret = {.x = 0, .y = 0};

        ret.x = b.x - a.x;
        ret.y = b.y - a.y;

        return ret;
}

