#include <SDL2/SDL.h>
#include <stdbool.h>
#include "map.h"
#include "vector.h"

#define TILE_HEIGHT 50
#define TILE_WIDTH 50

#define SCREEN_HEIGHT 24
#define SCREEN_WIDTH 32

#define RAD_BRIGHT 6
#define RAD_DIM 9
#define RAD_DARK 10

#define BRIGHT_LIGHT_ALPHA 0xff
#define DIM_LIGHT_ALPHA 0x66
#define DARK_LIGHT_ALPHA 0x33
#define NO_LIGHT_ALPHA 0x11

#define ABYSS 0
#define WALL 1
#define FLOOR 2
#define UNKNOWN 3

typedef uint8_t color_code;

typedef struct Color {
        color_code r;
        color_code g;
        color_code b;
        color_code a;
} Color;

typedef struct Stats {
        uint8_t hp;
} Stats;

typedef struct Entity {
        Point p;
        Color c;
        Stats s;
        bool collision;
} Entity;

typedef struct ColorMap {
        int height;
        int width;
        Color **data;
} ColorMap;

typedef struct World {
        Map map;
        Map alpha_mask;
        Map discovered_mask;
        Point camera;
        Entity player;
        Entity *enemies;
} World;


/* FUNCTION PROTOTYPES */

void normalize(Vector *v);
void calc_alpha_old(Map *alpha_map, Point p);
void calc_alpha(World *w, Point p);
Point *get_rad(Point center, uint8_t radius);
Entity newPlayer(int x, int y, bool c);
Color new_color(color_code r, color_code g, color_code b);
Entity newEnemy();
ColorMap newColorMap(int width, int height);
World newWorld(int width, int height, bool player_collision);
void move_up(Entity *e);
void move_down(Entity *e);
void move_right(Entity *e);
void move_left(Entity *e);
void init_world(World *w);
void set_tile(World *w, Point p, uint8_t b);
void apply_alpha_map(World *w, Map a);
void draw_entity(SDL_Renderer *r, Entity e);
void render_tile(SDL_Renderer *r, uint8_t b, Point p, Map a);
void render_world(SDL_Renderer *r, World w);
World load_map(char *filename);
void update_discovered(World *w);
uint8_t vec2alpha(Vector v);
void bresenham(Point src, Point dest, World *w);
void cam_track_player(World *w);
void draw_pts(World *w, Point *pts);

