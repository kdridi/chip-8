#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#define SCREEN_W 64
#define SCREEN_H 32
#define SCREEN_S 10

unsigned char chip8_screen[SCREEN_W * SCREEN_H] = {0};

void dissassemble_rom(unsigned char *data, long size);

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <rom file>\n", argv[0]);
        fprintf(stderr, "argc = %d\n", argc);
        return EXIT_FAILURE;
    }
    const char *rom_file = argv[1];

    FILE *rom = fopen(rom_file, "rb");
    if (!rom) {
        printf("Failed to open rom file %s\n", rom_file);
        return EXIT_FAILURE;
    }

    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    unsigned char *rom_data = malloc(rom_size);
    if (!rom_data) {
        printf("Failed to allocate memory for rom\n");
        return EXIT_FAILURE;
    }

    fread(rom_data, 1, rom_size, rom);
    fclose(rom);

    printf("ROM size: %ld\n", rom_size);

    dissassemble_rom(rom_data, rom_size);

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
                    if (chip8_screen[i * SCREEN_W + j]) {
                        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
                    } else {
                        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
                    }
                }
            }

            SDL_UpdateWindowSurface(window);

            for (int i = 0, k = 0; i < SCREEN_H; ++i) {
                for (int j = 0; j < SCREEN_W; ++j, ++k) {
                    chip8_screen[k] = (i + j) % 2;
                }
            }

        }
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    free(rom_data);

    return EXIT_SUCCESS;
}

//__attribute__((packed))

typedef union {
    unsigned short value;
    struct {
        unsigned char h : 8;
        unsigned char l : 8;
    };
    struct {
        union {
            unsigned char nn;
            struct {
                unsigned char n : 4;
                unsigned char y : 4;
            };
        };
        unsigned char x : 4;
        unsigned char f : 4;
    };
    struct {
        unsigned short nnn: 12;
        unsigned char _: 4;
    };
} opcode_t;

static_assert(sizeof(opcode_t) == 2, "opcode is not 2 bytes");

typedef void (*opcode_handler_t)(opcode_t);

// Execute machine language subroutine at address NNN
void chip8_cpu_0NNN(opcode_t opcode) {
    printf("SYS $%03X", opcode.nnn);
}

// Clear the screen
void chip8_cpu_00E0(opcode_t opcode) {
    printf("CLS");
}

// Return from a subroutine
void chip8_cpu_00EE(opcode_t opcode) {
    printf("RET");
}

void chip8_cpu_0___(opcode_t opcode) {
    switch (opcode.nn) {
        case 0x00e0:
            chip8_cpu_00E0(opcode);
            break;
        case 0x00ee:
            chip8_cpu_00EE(opcode);
            break;
        default:
            chip8_cpu_0NNN(opcode);
            break;
    }
}

// Jump to address NNN
void chip8_cpu_1NNN(opcode_t opcode) {
    printf("JP $%03X", opcode.nnn);
}

// Execute subroutine at address NNN
void chip8_cpu_2NNN(opcode_t opcode) {
    printf("CALL $%03X", opcode.nnn);
}

// Skip next instruction if the value of register VX equals NN
void chip8_cpu_3XNN(opcode_t opcode) {
    printf("SE V%X, $%02X", opcode.x, opcode.nn);
}

// Skip the following instruction if the value of register VX is not equal to NN
void chip8_cpu_4XNN(opcode_t opcode) {
    printf("SNE V%X, $%02X", opcode.x, opcode.nn);
}

// Skip the following instruction if the value of register VX equals the value of register VY
void chip8_cpu_5XY0(opcode_t opcode) {
    printf("SE V%X, V%X", opcode.x, opcode.y);
}

// Set register VX to NN
void chip8_cpu_6XNN(opcode_t opcode) {
    printf("LD V%X, $%02X", opcode.x, opcode.nn);
}

// Add NN to register VX
void chip8_cpu_7XNN(opcode_t opcode) {
    printf("ADD V%X, $%02X", opcode.x, opcode.nn);
}

// Set register VX to the value of register VY
void chip8_cpu_8XY0(opcode_t opcode) {
    printf("LD V%X, V%X", opcode.x, opcode.y);
}

// Set register VX to the value of register VX or VY
void chip8_cpu_8XY1(opcode_t opcode) {
    printf("OR V%X, V%X", opcode.x, opcode.y);
}

// Set register VX to the value of register VX and VY
void chip8_cpu_8XY2(opcode_t opcode) {
    printf("AND V%X, V%X", opcode.x, opcode.y);
}

// Set register VX to the value of register VX xor VY
void chip8_cpu_8XY3(opcode_t opcode) {
    printf("XOR V%X, V%X", opcode.x, opcode.y);
}

// Add the value of register VY to register VX. VF is set to 1 when there's a carry, and to 0 when there isn't
void chip8_cpu_8XY4(opcode_t opcode) {
    printf("ADD V%X, V%X", opcode.x, opcode.y);
}

// Subtract the value of register VY from register VX. VF is set to 0 when there's a borrow, and to 1 when there isn't
void chip8_cpu_8XY5(opcode_t opcode) {
    printf("SUB V%X, V%X", opcode.x, opcode.y);
}

// Store the value of register VY shifted right by one in register VX. VF is set to the least significant bit of VY before the shift. VY is unchanged.
void chip8_cpu_8XY6(opcode_t opcode) {
    printf("SHR V%X V%X", opcode.x, opcode.y);
}

// Set register VX to the value of register VY minus register VX. VF is set to 0 when there's a borrow, and to 1 when there isn't
void chip8_cpu_8XY7(opcode_t opcode) {
    printf("SUBN V%X, V%X", opcode.x, opcode.y);
}

// Store the value of register VY shifted left by one in register VX. VF is set to the most significant bit of VY before the shift. VY is unchanged.
void chip8_cpu_8XYE(opcode_t opcode) {
    printf("SHL V%X V%X", opcode.x, opcode.y);
}

opcode_handler_t handlers_8[] = {
        chip8_cpu_8XY0,
        chip8_cpu_8XY1,
        chip8_cpu_8XY2,
        chip8_cpu_8XY3,
        chip8_cpu_8XY4,
        chip8_cpu_8XY5,
        chip8_cpu_8XY6,
        chip8_cpu_8XY7,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        chip8_cpu_8XYE,
        NULL,
};

void chip8_cpu_8___(opcode_t opcode) {
    opcode_handler_t h = handlers_8[opcode.n];
    if (h) {
        h(opcode);
    } else {
        fprintf(stderr, "Unknown opcode: 0x%04x\n", opcode);
    }
}

// Skip the following instruction if the value of register VX is not equal to the value of register VY
void chip8_cpu_9XY0(opcode_t opcode) {
    printf("SNE V%X, V%X", opcode.x, opcode.y);
}

// Store memory address NNN in register I
void chip8_cpu_ANNN(opcode_t opcode) {
    printf("LD I, $%03X", opcode.nnn);
}

// Jump to address NNN plus the value of register V0
void chip8_cpu_BNNN(opcode_t opcode) {
    printf("JP V0, $%03X", opcode.nnn);
}

// Set register VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
void chip8_cpu_CXNN(opcode_t opcode) {
    printf("RND V%X, $%02X", opcode.x, opcode.nn);
}

// Draw a sprite at coordinate (VX, VY) with N bytes of sprite data starting at the address stored in register I
void chip8_cpu_DXYN(opcode_t opcode) {
    printf("DRW V%X, V%X, $%02X", opcode.x, opcode.y, opcode.n);
}

// Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
void chip8_cpu_EX9E(opcode_t opcode) {
    printf("SKP V%X", opcode.x);
}

// Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
void chip8_cpu_EXA1(opcode_t opcode) {
    printf("SKNP V%X", opcode.x);
}

void chip8_cpu_E___(opcode_t opcode) {
    switch (opcode.nn) {
        case 0x9e: {
            chip8_cpu_EX9E(opcode);
            break;
        }
        case 0xa1: {
            chip8_cpu_EXA1(opcode);
            break;
        }
        default: {
            fprintf(stderr, "Unknown opcode: 0x%04x\n", opcode);
            break;
        }
    }
}

// Store the current value of the delay timer in register VX
void chip8_cpu_FX07(opcode_t opcode) {
    printf("LD V%X, DT", opcode.x);
}

// Wait for a key press, store the hex value of the key in register VX
void chip8_cpu_FX0A(opcode_t opcode) {
    printf("LD V%X, K", opcode.x);
}

// Set the delay timer to the value of register VX
void chip8_cpu_FX15(opcode_t opcode) {
    printf("LD DT, V%X", opcode.x);
}

// Set the sound timer to the value of register VX
void chip8_cpu_FX18(opcode_t opcode) {
    printf("LD ST, V%X", opcode.x);
}

// Add the value of register VX to register I
void chip8_cpu_FX1E(opcode_t opcode) {
    printf("ADD I, V%X", opcode.x);
}

// Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
void chip8_cpu_FX29(opcode_t opcode) {
    printf("LD F, V%X", opcode.x);
}

// Store the binary-coded decimal equivalent of the value stored in register VX at address I, I+1, and I+2
void chip8_cpu_FX33(opcode_t opcode) {
    printf("LD B, V%X", opcode.x);
}

// Store the values of registers V0 to VX inclusive in memory starting at address I, I+1, and I+2
void chip8_cpu_FX55(opcode_t opcode) {
    printf("LD [I], V%X", opcode.x);
}

// Fill registers V0 to VX inclusive with the values stored in memory starting at address I, I+1, and I+2
void chip8_cpu_FX65(opcode_t opcode) {
    printf("LD V%X, [I]", opcode.x);
}


void chip8_cpu_F___(opcode_t opcode) {
    switch (opcode.nn) {
        case 0x07: {
            chip8_cpu_FX07(opcode);
            break;
        }
        case 0x0a: {
            chip8_cpu_FX0A(opcode);
            break;
        }
        case 0x15: {
            chip8_cpu_FX15(opcode);
            break;
        }
        case 0x18: {
            chip8_cpu_FX18(opcode);
            break;
        }
        case 0x1e: {
            chip8_cpu_FX1E(opcode);
            break;
        }
        case 0x29: {
            chip8_cpu_FX29(opcode);
            break;
        }
        case 0x33: {
            chip8_cpu_FX33(opcode);
            break;
        }
        case 0x55: {
            chip8_cpu_FX55(opcode);
            break;
        }
        case 0x65: {
            chip8_cpu_FX65(opcode);
            break;
        }
        default: {
            fprintf(stderr, "Unknown opcode: 0x%04x\n", opcode);
            abort();
        }
    }
}

opcode_handler_t handlers[] = {
        chip8_cpu_0___,
        chip8_cpu_1NNN,
        chip8_cpu_2NNN,
        chip8_cpu_3XNN,
        chip8_cpu_4XNN,
        chip8_cpu_5XY0,
        chip8_cpu_6XNN,
        chip8_cpu_7XNN,
        chip8_cpu_8___,
        chip8_cpu_9XY0,
        chip8_cpu_ANNN,
        chip8_cpu_BNNN,
        chip8_cpu_CXNN,
        chip8_cpu_DXYN,
        chip8_cpu_E___,
        chip8_cpu_F___,
};

void dissassemble_rom(unsigned char *data, long size) {
    long pc = 0;

    while (pc < size) {
        opcode_t opcode = {0};
        printf("%04lx: ", 0x200 + pc);

        opcode.l = data[pc++];
        opcode.h = data[pc++];
        printf("%04x: ", opcode.value);

        handlers[opcode.f](opcode);

        printf("\n");
    }
}
