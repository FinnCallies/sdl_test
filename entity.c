#include "entity.h"

Entity new_entity(Point position, bool collision)
{
        Entity ret = {.position = position, .color = cc2Color(0xffffff), .collision = collision};

        return ret;
}

void set_position(Entity entity, Point pos)
{
        entity.position = pos;
}

void set_color(Entity entity, Color color)
{
        entity.color = color;
}

void set_color_val(Entity entity, color_code color)
{
        entity.color = cc2Color(color);
}

void set_collision(Entity entity, bool collision)
{
        entity.collision = collision;
}

void move_up(Entity *entity) 
{
        (*entity).position.y--;
}

void move_down(Entity *entity) 
{
        (*entity).position.y++;
}

void move_right(Entity *entity) 
{
        (*entity).position.x++;
}

void move_left(Entity *entity) 
{
        (*entity).position.x--;
}

