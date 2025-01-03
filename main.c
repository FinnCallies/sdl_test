#include "utils.h"

int main(int argc, char *argv[]) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                printf("SDL konnte nicht initialisiert werden: %s\n", SDL_GetError());
                return 1;
        }

        SDL_Window* window = SDL_CreateWindow("2D Top-Down Spiel",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH * TILE_WIDTH, SCREEN_HEIGHT * TILE_HEIGHT,
                                          SDL_WINDOW_SHOWN);

        if (!window) {
                printf("Fenster konnte nicht erstellt werden: %s\n", SDL_GetError());
                SDL_Quit();
                return 1;
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        bool running = true;
        SDL_Event event;

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        World world;
        if(argc > 1) {
                FILE *fptr = fopen(argv[1], "r");
                char *line;
                size_t len = 0;
                int l_idx = 0;
                int idx = 0;
                int h = SCREEN_HEIGHT * 2;
                int w = SCREEN_WIDTH * 2;

                if(getline(&line, &len, fptr) != -1 && line[0] != ' ') {
                        char *space = strchr(line, ' ');
                        *space = '\0';

                        w = atoi(line);
                        h = atoi(space + 1);
                }

                world = newWorld(w, h, true);

                while(getline(&line, &len, fptr) != -1) {
                        idx = 0;
                        
                        while(line[idx] != '\n') {
                                world.map.data[l_idx][idx] = line[idx] - '0';
                                idx++;
                        }
                        l_idx++;
                }

                printf("Map loaded: [ %d, %d ]\n", idx, l_idx);

                fclose(fptr);
        } else {
                world = newWorld(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, true);
                init_world(&world);
        }

        while (running) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                running = false;
                        }
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                const Uint8* state = SDL_GetKeyboardState(NULL);
                Point temp_save = world.player.p;
                if (state[SDL_SCANCODE_W]) {
                        move_up(&world.player);
                        if(world.player.p.y < 0)
                                world.player.p.y = 0;
                        else if(world.map.data[world.player.p.y][world.player.p.x] == WALL)
                                move_down(&world.player);
                }
                if (state[SDL_SCANCODE_S]) {
                        move_down(&world.player);
                        if(world.player.p.y > world.map.height - 1)
                                world.player.p.y = world.map.height - 1;
                        else if(world.map.data[world.player.p.y][world.player.p.x] == WALL)
                                move_up(&world.player);
                }
                if (state[SDL_SCANCODE_A]) {
                        move_left(&world.player);
                        if(world.player.p.x < 0)
                                world.player.p.x = 0;
                        else if(world.map.data[world.player.p.y][world.player.p.x] == WALL)
                                move_right(&world.player);
                }
                if (state[SDL_SCANCODE_D]) {
                        move_right(&world.player);
                        if(world.player.p.x > world.map.width - 1)
                                world.player.p.x = world.map.width - 1;
                        else if(world.map.data[world.player.p.y][world.player.p.x] == WALL)
                                move_left(&world.player);
                }

                SDL_ResetKeyboard();

                Vector cam2p = pts2vec(world.camera, world.player.p);
                normalize(&cam2p);

                if(cam2p.x < 4)
                        world.camera.x--;
                else if(cam2p.x > SCREEN_WIDTH - 4 - 1)
                        world.camera.x++;
                else if(cam2p.y < 4)
                        world.camera.y--;
                else if(cam2p.y > SCREEN_HEIGHT - 4 - 1)
                        world.camera.y++;

                Point temp = newPoint(world.player.p.x - world.camera.x, world.player.p.y - world.camera.y);
                calc_alpha(&world.alpha_mask, temp);

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
