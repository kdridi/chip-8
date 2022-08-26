#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include "chip8.h"

Uint32 chip8_timer_callback(Uint32 interval, void *param) {
    // printf("timer callback\n");
    chip8_fetch();
    chip8_execute();
    if (chip8.cpu.delay_timer == 0) {
        chip8.cpu.delay_timer = 60;
    } else {
        chip8.cpu.delay_timer--;
    }
    if (chip8.cpu.sound_timer > 0) {
        char byte = '\a';
        write(STDOUT_FILENO, &byte, 1);
        chip8.cpu.sound_timer--;
    }
    return interval;
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <rom file>\n", argv[0]);
        fprintf(stderr, "argc = %d\n", argc);
        return EXIT_FAILURE;
    }

    chip8_init();
    if (chip8_rom_load(argv[1]) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    uint16_t keymap[16] = {
        SDLK_x,
        SDLK_1, SDLK_2, SDLK_3,
        SDLK_a, SDLK_z, SDLK_e,
        SDLK_q, SDLK_s, SDLK_d,
        SDLK_w, SDLK_c,
        SDLK_4, SDLK_r, SDLK_f, SDLK_v,
    };

    // chip8_disassemble();

    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL_Init failed: %s\n", SDL_GetError());
            return EXIT_FAILURE;
        }
        SDL_Window *window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W * SCREEN_S, SCREEN_H * SCREEN_S, SDL_WINDOW_SHOWN);
        if (!window) {
            printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
            return EXIT_FAILURE;
        }
        SDL_Surface *surface = SDL_GetWindowSurface(window);
        if (!surface) {
            printf("SDL_GetWindowSurface failed: %s\n", SDL_GetError());
            return EXIT_FAILURE;
        }
        SDL_UpdateWindowSurface(window);
        SDL_TimerID timer = SDL_AddTimer(1000 / 600, chip8_timer_callback, NULL);
        SDL_Event event;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F1) {
                    chip8_reset();
                } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                    int8_t key = -1;
                    for (int i = 0; i < 16; i++) {
                        if (event.key.keysym.sym == keymap[i]) {
                            key = i;
                            break;
                        }
                    }
                    if (key >= 0) {
                        chip8.keys[key] = event.type == SDL_KEYDOWN;
                    }
                    chip8.key_pressed = -1;
                    if (event.type == SDL_KEYDOWN) {
                        chip8.key_pressed = key;
                    }
                }
            }

            if (chip8.screen_redraw) {
                chip8.screen_redraw = false;
                for (int i = 0; i < SCREEN_H; ++i) {
                    for (int j = 0; j < SCREEN_W; ++j) {
                        SDL_Rect rect = {j * SCREEN_S, i * SCREEN_S, SCREEN_S, SCREEN_S};
                        if (chip8.screen[i * SCREEN_W + j]) {
                            SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
                        } else {
                            SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
                        }
                    }
                }
                SDL_UpdateWindowSurface(window);
            }


            /*
            for (int i = 0, k = 0; i < SCREEN_H; ++i) {
                for (int j = 0; j < SCREEN_W; ++j, ++k) {
                    chip8.screen[k] = (i + j) % 2;
                }
            }
             */

        }
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return EXIT_SUCCESS;
}












