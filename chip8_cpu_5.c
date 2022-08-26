//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

// Skip the following instruction if the value of register VX equals the value of register VY
void chip8_cpu_5XY0() {
    // printf("SE V%X, V%X", chip8.cpu.opcode.x, chip8.cpu.opcode.y);
    // fflush(stdout);
    if (chip8.cpu.v[chip8.cpu.opcode.x] == chip8.cpu.v[chip8.cpu.opcode.y]) {
        chip8.cpu.pc += 2;
    }
}
