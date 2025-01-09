#include <stdlib.h>
#include <stdint.h>

typedef struct Map {
        int height;
        int width;
        uint8_t **data;
} Map;

/* FUNCTION PROTOTYPES */
Map new_map(int width, int height);
void init_map(Map *m, uint8_t v);

