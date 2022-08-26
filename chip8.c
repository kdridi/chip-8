//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stddef.h>

chip8_t chip8 = {
    .rom_file = {
        .name = NULL,
        .length = 0
    },
    .ram = {0},
    .screen = {0},
    .cpu = {
        .opcode = {0},
        .pc = 0x200,
        .i = 0,
        .v = {0},
        .stack = {0},
        .sp = 0
    }
};
