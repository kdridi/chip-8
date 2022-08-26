//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>

// Skip the following instruction if the value of register VX equals the value of register VY
void chip8_cpu_5XY0() {
    printf("SE V%X, V%X", chip8.cpu.opcode.x, chip8.cpu.opcode.y);
}
