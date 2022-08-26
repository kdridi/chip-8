#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "chip8.h"

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <rom file>\n", argv[0]);
        fprintf(stderr, "argc = %d\n", argc);
        return EXIT_FAILURE;
    }

    if (chip8_rom_load(argv[1]) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    chip8_disassemble();

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
        SDL_Event event;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
                if (event.type == SDL_KEYDOWN) {
                    printf("Key down: %d\n", event.key.keysym.sym);
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        quit = true;
                    }
                }
            }

            for (int i = 0; i < SCREEN_H; ++i) {
                for (int j = 0; j < SCREEN_W; ++j) {
                    SDL_Rect rect = {j * SCREEN_S, i * SCREEN_S, SCREEN_S, SCREEN_S};
                    if (chip8.screen[i * SCREEN_W + j]) {
                        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
                    } else {
                        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
                    }
                }
            }

            SDL_UpdateWindowSurface(window);

            for (int i = 0, k = 0; i < SCREEN_H; ++i) {
                for (int j = 0; j < SCREEN_W; ++j, ++k) {
                    chip8.screen[k] = (i + j) % 2;
                }
            }

        }
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return EXIT_SUCCESS;
}












