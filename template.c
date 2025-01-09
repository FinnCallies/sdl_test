#include "utils.h"

int main(int argc, char *argv[])
{
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
                printf("SDL konnte nicht initialisiert werden: %s\n", SDL_GetError());
                return 1;
        }

        SDL_Window *window = SDL_CreateWindow("2D Top-Down Spiel",
                                              SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED,
                                              SCREEN_WIDTH * TILE_WIDTH, SCREEN_HEIGHT * TILE_HEIGHT,
                                              SDL_WINDOW_SHOWN);

        if (!window)
        {
                printf("Fenster konnte nicht erstellt werden: %s\n", SDL_GetError());
                SDL_Quit();
                return 1;
        }

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        bool running = true;
        SDL_Event event;

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        World world;
        world = newWorld(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, true);
        init_world(&world);

        // Point temp = newPoint(world.player.p.x - world.camera.x, world.player.p.y - world.camera.y);
        // calc_alpha_old(&world.alpha_mask, temp);
        // update_discovered(&world);
        for(int i = 0; i < SCREEN_HEIGHT; i++) {
                for(int j = 0; j < SCREEN_WIDTH; j++) {
                        world.alpha_mask.data[i][j] = 0xff;
                }
        }

        for(int i = 0; i < world.map.height; i++) {
                for(int j = 0; j < world.map.width; j++) {
                        world.map.data[i][j] = FLOOR;
                }
        }

        for(int i = 0; i < world.map.height; i++) 
                for(int j = 0; j < world.map.width; j++)
                        world.discovered_mask.data[i][j] = 0x01;

        while (running)
        {
                while (SDL_PollEvent(&event))
                {
                        if (event.type == SDL_QUIT)
                        {
                                running = false;
                        }
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                const Uint8 *state = SDL_GetKeyboardState(NULL);
                if (state[SDL_SCANCODE_W])
                {
                        move_up(&world.player);
                        if (world.player.p.y < 0)
                                world.player.p.y = 0;
                        else if (world.map.data[world.player.p.y][world.player.p.x] == WALL)
                                move_down(&world.player);
                }
                if (state[SDL_SCANCODE_S])
                {
                        move_down(&world.player);
                        if (world.player.p.y > world.map.height - 1)
                                world.player.p.y = world.map.height - 1;
                        else if (world.map.data[world.player.p.y][world.player.p.x] == WALL)
                                move_up(&world.player);
                }
                if (state[SDL_SCANCODE_A])
                {
                        move_left(&world.player);
                        if (world.player.p.x < 0)
                                world.player.p.x = 0;
                        else if (world.map.data[world.player.p.y][world.player.p.x] == WALL)
                                move_right(&world.player);
                }
                if (state[SDL_SCANCODE_D])
                {
                        move_right(&world.player);
                        if (world.player.p.x > world.map.width - 1)
                                world.player.p.x = world.map.width - 1;
                        else if (world.map.data[world.player.p.y][world.player.p.x] == WALL)
                                move_left(&world.player);
                }

                SDL_ResetKeyboard();

                cam_track_player(&world);

                for(int i = 0; i < world.map.height; i++)
                        for(int j = 0; j < world.map.width; j++)
                                world.map.data[i][j] = FLOOR;

                draw_pts(&world, get_rad(world.player.p, 4));

                Point temp = newPoint(world.player.p.x - world.camera.x, world.player.p.y - world.camera.y);
                // calc_alpha_old(&world.alpha_mask, temp);

                // apply_alpha_map(&world, alpha_map);
                render_world(renderer, world);

                SDL_RenderPresent(renderer);

                SDL_Delay(32);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}
