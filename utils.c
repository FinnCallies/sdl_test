#include "utils.h"

void normalize(Vector *v)
{
        if(v->x < 0)
                v->x = v->x * -1;
        if(v->y < 0)
                v->y = v->y * -1;
}

// calculate distance from a to b as Vector
Vector pts2vec(Point a, Point b) 
{
        Vector ret = {.x = 0, .y = 0};

        ret.x = b.x - a.x;
        ret.y = b.y - a.y;

        return ret;
}

void calc_alpha(Map *alpha_map, Point p)
{
        for(int i = 0; i < alpha_map->height; i++) {
                for(int j = 0; j < alpha_map->width; j++) {
                        Vector v = pts2vec(p, newPoint(j, i));
                        normalize(&v);

                        if(v.x < RAD_BRIGHT && v.y < RAD_BRIGHT && v.x + v.y < RAD_BRIGHT + 1)
                                (alpha_map->data)[i][j] = BRIGHT_LIGHT_ALPHA;
                        else if(v.x < RAD_DIM && v.y < RAD_DIM && v.x + v.y < RAD_DIM + 1)
                                (alpha_map->data)[i][j] = DIM_LIGHT_ALPHA;
                        else if(v.x < RAD_DARK && v.y < RAD_DARK && v.x + v.y < RAD_DARK + 1)
                                (alpha_map->data)[i][j] = DARK_LIGHT_ALPHA;
                        else
                                (alpha_map->data)[i][j] = NO_LIGHT_ALPHA;
                }
        }
}

Map new_map(int width, int height)
{
        Map ret = {.width = width, .height = height, .data = NULL};

        ret.data = (uint8_t **) calloc(height, sizeof(uint8_t *));
        for(int i = 0; i < height; i++) 
                ret.data[i] = (uint8_t *) calloc(width, sizeof(uint8_t));

        return ret;
}

ColorMap newColorMap(int width, int height)
{
        ColorMap ret = {.width = width, .height = height, .data = NULL};

        ret.data = (Color **) calloc(height, sizeof(Color *));
        for(int i = 0; i < height; i++) 
                ret.data[i] = (Color *) calloc(width, sizeof(Color));

        return ret;
}

Point newPoint(int x, int y)
{
        Point ret = {.x = x, .y = y};
        return ret;
}

Color new_color(color_code r, color_code g, color_code b)
{
        Color ret = {.r = r, .g = g, .b = b};

        return ret;
}

Entity newPlayer(int x, int y, bool c) {
        Entity ret = {
                .p = newPoint(x, y),
                .c = {.r = 0x00, .g = 0x00, .b = 0xff},
                .s = {.hp = 20},
                .collision = c,
        };

        return ret;
}

Entity newEnemy() {
        Entity ret = {
                .p = {.x = 1 * TILE_WIDTH, .y = 1 * TILE_HEIGHT},
                .c = {.r = 0xff, .g = 0x00, .b = 0x00},
                .s = {.hp = 10},
                .collision = true,
        };

        return ret;
}

World newWorld(int width, int height, bool player_collision)
{
        World ret = {
                .map = new_map(width, height), 
                .alpha_mask = new_map(SCREEN_WIDTH, SCREEN_HEIGHT),
                .camera = newPoint(1, 0), 
                .player = newPlayer(2, 2, player_collision) 
        };

        return ret;
}

void move_up(Entity *e)
{
        (*e).p.y--;
}

void move_down(Entity *e)
{
        (*e).p.y++;
}

void move_right(Entity *e)
{
        (*e).p.x++;
}

void move_left(Entity *e)
{
        (*e).p.x--;
}

void set_tile(World *w, Point p, uint8_t b)
{
        if(p.x < 0 || p.y < 0 || p.x >= (*w).map.width || p.y >= (*w).map.height)
                return;

        (*w).map.data[p.y][p.x] = b;
}

void init_world(World *w)
{
        for(int i = 0; i < (*w).map.height; i++)
                for(int j = 0; j < (*w).map.width; j++)
                        (*w).map.data[i][j] = FLOOR;
}

/*
void apply_alpha_map(World *w, Map a)
{
        for(int i = 0; i < a.height; i++) {
                if((i + (*w).camera.y) >= (*w).map.height || (i + (*w).camera.y) < 0)
                        continue;

                for(int j = 0; j < a.width; j++) {
                        if((j + (*w).camera.x) >= (*w).map.width || (j + (*w).camera.x) < 0)
                                continue;

                        (*w).map.data[i + (*w).camera.y][j + (*w).camera.x].a = a.data[i][j];
                }
        }
}
*/

Color block2color(uint8_t b)
{
        Color ret = {.r = 0xff, .g = 0x00, .b = 0x00};

        switch(b) {
                case ABYSS:
                        ret.r = 0x11;
                        ret.g = 0x11;
                        ret.b = 0x11;
                        break;
                case WALL:
                        ret.r = 0xd4;
                        ret.g = 0xbe;
                        ret.b = 0x90;
                        break;
                case FLOOR:
                        ret.r = 0x2d;
                        ret.g = 0x5e;
                        ret.b = 0x13;
                        break;
                default:
                        break;
        }

        return ret;
}

void draw_entity(SDL_Renderer *r, Entity e) 
{
        SDL_SetRenderDrawColor(r, e.c.r, e.c.g, e.c.b, 255);
        SDL_Rect rect = {e.p.x * TILE_WIDTH + 10, e.p.y *TILE_HEIGHT + 10, TILE_HEIGHT - 20, TILE_WIDTH - 20};
        SDL_RenderFillRect(r, &rect);
}

void render_tile(SDL_Renderer *r, uint8_t b, Point p, Map a) 
{
        Color c = block2color(b);

        SDL_SetRenderDrawColor(r, c.r, c.g, c.b, a.data[p.y][p.x]);
        SDL_Rect rect = {p.x * TILE_WIDTH + 1, p.y * TILE_HEIGHT + 1, TILE_HEIGHT - 2, TILE_WIDTH - 2};
        SDL_RenderFillRect(r, &rect);
}

void render_entity_in_respect_to(SDL_Renderer *r, Entity e, Point respect)
{
        SDL_SetRenderDrawColor(r, e.c.r, e.c.g, e.c.b, 0xff);
        SDL_Rect rect = {(e.p.x - respect.x) * TILE_WIDTH + 10, (e.p.y - respect.y) * TILE_HEIGHT + 10, TILE_HEIGHT - 20, TILE_WIDTH - 20};
        SDL_RenderFillRect(r, &rect);
}

void render_world(SDL_Renderer *r, World w)
{
        for(int i = w.camera.y; (i - w.camera.y) < SCREEN_HEIGHT && i < w.map.height; i++) {
                if(i < 0)
                        continue;

                for(int j = w.camera.x; (j - w.camera.x) < SCREEN_WIDTH && j < w.map.width; j++) {
                        if(j < 0)
                                continue;

                        render_tile(r, w.map.data[i][j], newPoint(j - w.camera.x, i - w.camera.y), w.alpha_mask);
                }
        }

        render_entity_in_respect_to(r, w.player, w.camera);
}

