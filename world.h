#include "map.h"
#include "entity.h"

typedef struct {
        Map map;
        Map alpha_mask;
        Map discovered_mask;
        Point camera;
        Entity player;
        Entity *enemies;
} World;

