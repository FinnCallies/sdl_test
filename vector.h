#include "point.h"

typedef struct Vector {
        int x;
        int y;
} Vector;

void normalize(Vector *v);
Vector pts2vec(Point a, Point b);

