#include "utils.h"


// calculate distance from a to b as Vector

Point *get_rad(Point center, uint8_t radius)
{
        int idx = 0;
        Point *ret = (Point *) calloc(radius * 8, sizeof(Point));

        for(int i = 0; i < radius * 8; i++) {
                ret[i] = newPoint(-1, -1);
        }

        for(int i = center.y - radius; i < center.y + radius; i++) {
                if(i < 0)
                        continue;

                for(int j = center.x - radius; j < center.x + radius; j++) {
                        if(j < 0)
                                continue;

                        Vector v = pts2vec(center, newPoint(j, i));
                        normalize(&v);
                        if(v.x == RAD_DARK - 1 || v.y == RAD_DARK - 1 || v.x + v.y == RAD_DARK) {
                                ret[idx].x = j;
                                ret[idx].y = i;

                                idx++;
                        }
                }
        }

        return ret;
}

void calc_alpha_old(Map *alpha_map, Point p) 
{
        for (int i = 0; i < alpha_map->height; i++)
        {
                for (int j = 0; j < alpha_map->width; j++)
                {
                        Point np = newPoint(j, i);
                        Vector v = pts2vec(p, np);
                        normalize(&v);

                        // TBD bresenham

                        if (v.x < RAD_BRIGHT && v.y < RAD_BRIGHT && v.x + v.y < RAD_BRIGHT + 1)
                                (alpha_map->data)[i][j] = BRIGHT_LIGHT_ALPHA;
                        else if (v.x < RAD_DIM && v.y < RAD_DIM && v.x + v.y < RAD_DIM + 1)
                                (alpha_map->data)[i][j] = DIM_LIGHT_ALPHA;
                        else if (v.x < RAD_DARK && v.y < RAD_DARK && v.x + v.y < RAD_DARK + 1)
                                (alpha_map->data)[i][j] = DARK_LIGHT_ALPHA;
                        else
                                (alpha_map->data)[i][j] = NO_LIGHT_ALPHA;
                }
        }
}

void calc_alpha(World *w, Point p)
{
        /*
        */

        Point *vision_border = get_rad(p, RAD_DARK);

        for(int i = 0; vision_border[i].x != -1; i++)
                printf("[ %d, %d ]\n", vision_border[i].x, vision_border[i].y);

        for(int i = 0; i < RAD_DARK * 8 && (vision_border[i].x != -1 && vision_border[i].y != -1);
i++) {
                bresenham(p, vision_border[i], w);
        }
}

ColorMap newColorMap(int width, int height)
{
        ColorMap ret = {.width = width, .height = height, .data = NULL};

        ret.data = (Color **)calloc(height, sizeof(Color *));
        for (int i = 0; i < height; i++)
                ret.data[i] = (Color *)calloc(width, sizeof(Color));

        return ret;
}


Color new_color(color_code r, color_code g, color_code b)
{
        Color ret = {.r = r, .g = g, .b = b};

        return ret;
}

Entity newPlayer(int x, int y, bool c)
{
        Entity ret = {
            .p = newPoint(x, y),
            .c = {.r = 0x00, .g = 0x00, .b = 0xff},
            .s = {.hp = 20},
            .collision = c,
        };

        return ret;
}

Entity newEnemy()
{
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
            .discovered_mask = new_map(width, height),
            .camera = newPoint(1, 0),
            .player = newPlayer(2, 2, player_collision)};

        init_map(&ret.discovered_mask, 0);

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
        if (p.x < 0 || p.y < 0 || p.x >= (*w).map.width || p.y >= (*w).map.height)
                return;

        (*w).map.data[p.y][p.x] = b;
}

void init_world(World *w)
{
        for (int i = 0; i < (*w).map.height; i++)
                for (int j = 0; j < (*w).map.width; j++)
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

        switch (b)
        {
        case UNKNOWN:
                ret.r = 0xaa;
                ret.g = 0xaa;
                ret.b = 0xaa;
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

void render_rect(SDL_Renderer *r, Color color, Point position, int size_x, int size_y)
{
        SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
        SDL_Rect rect = {position.x, position.y, size_y, size_x};
        SDL_RenderFillRect(r, &rect);
}

void draw_entity(SDL_Renderer *r, Entity e)
{
        SDL_SetRenderDrawColor(r, e.c.r, e.c.g, e.c.b, 255);
        SDL_Rect rect = {e.p.x * TILE_WIDTH + 10, e.p.y * TILE_HEIGHT + 10, TILE_HEIGHT - 20, TILE_WIDTH - 20};
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
        for (int i = w.camera.y; (i - w.camera.y) < SCREEN_HEIGHT && i < w.map.height; i++)
        {
                if (i < 0)
                        continue;

                for (int j = w.camera.x; (j - w.camera.x) < SCREEN_WIDTH && j < w.map.width; j++)
                {
                        if (j < 0)
                                continue;

                        if (w.discovered_mask.data[i][j] > 0)
                                render_tile(r, w.map.data[i][j], newPoint(j - w.camera.x, i - w.camera.y), w.alpha_mask);
                        else
                                render_tile(r, UNKNOWN, newPoint(j - w.camera.x, i - w.camera.y), w.alpha_mask);
                }
        }

        render_entity_in_respect_to(r, w.player, w.camera);
}

/*
void render_hud(SDL_Renderer *r, World w)
{
        Color hp = {.r = 0xff, .g = 0x00, .b = 0x00, .a = 0xff};
        Point pos_hp = newPoint()
            render_rect(r, hp, )
}
*/

World load_map(char *filename)
{
        World world;
        FILE *fptr = fopen(filename, "r");
        char *line;
        size_t len = 0;
        int l_idx = 0;
        int idx = 0;
        int h = SCREEN_HEIGHT * 2;
        int w = SCREEN_WIDTH * 2;

        if (getline(&line, &len, fptr) != -1 && line[0] != ' ')
        {
                char *space = strchr(line, ' ');
                *space = '\0';

                w = atoi(line);
                h = atoi(space + 1);
        }

        world = newWorld(w, h, true);

        while (getline(&line, &len, fptr) != -1)
        {
                idx = 0;

                while (line[idx] != '\n')
                {
                        world.map.data[l_idx][idx] = line[idx] - '0';
                        idx++;
                }
                l_idx++;
        }

        printf("Map loaded: [ %d, %d ]\n", idx, l_idx);

        fclose(fptr);

        return world;
}

void update_discovered(World *w)
{
        for (int i = 0; i < (*w).alpha_mask.height; i++)
                for (int j = 0; j < (*w).alpha_mask.width; j++)
                        if ((*w).alpha_mask.data[i][j] > NO_LIGHT_ALPHA && (i + (*w).camera.y) >= 0 && (i + (*w).camera.y) < (*w).map.height && (j + (*w).camera.x) >= 0 && (j + (*w).camera.x) < (*w).map.width)
                                (*w).discovered_mask.data[i + (*w).camera.y][j + (*w).camera.x] = 1;
}

uint8_t vec2alpha(Vector v)
{
        if (v.x < RAD_BRIGHT && v.y < RAD_BRIGHT && v.x + v.y < RAD_BRIGHT + 1)
                return BRIGHT_LIGHT_ALPHA;
        else if (v.x < RAD_DIM && v.y < RAD_DIM && v.x + v.y < RAD_DIM + 1)
                return DIM_LIGHT_ALPHA;
        else if (v.x < RAD_DARK && v.y < RAD_DARK && v.x + v.y < RAD_DARK + 1)
                return DARK_LIGHT_ALPHA;
        else
                return NO_LIGHT_ALPHA;
}

void bresenham(Point src, Point dest, World *w)
{
        int dx =  abs(dest.x - src.x), sx = src.x < dest.x ? 1 : -1;
        int dy = -abs(dest.y - src.y), sy = src.y < dest.y ? 1 : -1;
        int err = dx + dy, e2; /* error value e_xy */
        Point origin = src;

        while (1) {
                if((*w).map.data[src.y][src.x] == WALL)
                        return;

                // set pixel in map
                // (*w).alpha_mask.data[src.y - (*w).camera.y][src.x - (*w).camera.x]
                // = vec2alpha(pts2vec(origin, src));
                

                if (src.x == dest.x && src.y == dest.y) 
                        break;
                e2 = 2 * err;

                if (e2 > dy) { 
                        err += dy; 
                        src.x += sx; 
                } /* e_xy+e_x > 0 */

                if (e2 < dx) { 
                        err += dx; 
                        src.y += sy; 
                } /* e_xy+e_y < 0 */
        }
}

void cam_track_player(World *w)
{
        Vector cam2p = pts2vec((*w).camera, (*w).player.p);
        normalize(&cam2p);

        if (cam2p.x < 4)
                (*w).camera.x--;
        else if (cam2p.x > SCREEN_WIDTH - 4 - 1)
                (*w).camera.x++;
        else if (cam2p.y < 4)
                (*w).camera.y--;
        else if (cam2p.y > SCREEN_HEIGHT - 4 - 1)
                (*w).camera.y++;
}

void draw_pts(World *w, Point *pts)
{
        for(int i = 0; pts[i].x != -1; i++) {
                (*w).map.data[pts[i].y][pts[i].x] = WALL;
        }
}

