#include <stdlib.h>
#include <stdint.h>
#include "point.h"

#ifndef MAP_H
#define MAP_H

typedef struct {
        int height;
        int width;
        uint8_t **data;
} Map;

/* FUNCTION PROTOTYPES */
Map new_map(int width, int height);
void set_map(Map *m, uint8_t v);
void set_field(Map *map, Point field, uint8_t value);
void set_fields(Map *map, Point *fields, int count, uint8_t value);

#endif /* MAP_H */

