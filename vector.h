#include "point.h"

typedef struct {
        int x;
        int y;
} Vector;

void normalize(Vector *v);
Vector pts2vec(Point a, Point b);

