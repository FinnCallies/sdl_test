#include <SDL2/SDL.h>
#include <stdbool.h>
#include "vector.h"
#include "world.h"

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

typedef struct Stats {
        uint8_t hp;
} Stats;

typedef struct ColorMap {
        int height;
        int width;
        Color **data;
} ColorMap;

/* FUNCTION PROTOTYPES */

void calc_alpha_old(Map *alpha_map, Point p);
void calc_alpha(World *w, Point p);
Entity newPlayer(int x, int y, bool c);
Entity newEnemy();
World newWorld(int width, int height, bool player_collision);
void init_world(World *w);
void set_tile(World *w, Point p, uint8_t b);
void apply_alpha_map(World *w, Map a);
void draw_entity(SDL_Renderer *r, Entity e);
void render_tile(SDL_Renderer *r, uint8_t b, Point p, Map a);
void render_world(SDL_Renderer *r, World w);
World load_map(char *filename);
void update_discovered(World *w);
uint8_t vec2alpha(Vector v);
void cam_track_player(World *w);
void draw_pts(World *w, Point *pts);

