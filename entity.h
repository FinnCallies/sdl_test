#include "color.h"
#include "point.h"
#include <stdbool.h>

typedef struct {
        Point position;
        Color color;
        bool collision;
} Entity;

Entity new_entity(Point position, bool collision);
void set_position(Entity entity, Point pos);
void set_color(Entity entity, Color color);
void set_color_val(Entity entity, color_code color);
void set_collision(Entity entity, bool collision);
void move_up(Entity *entity);
void move_down(Entity *entity);
void move_right(Entity *entity);
void move_left(Entity *entity);

