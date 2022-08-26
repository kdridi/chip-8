//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>

// Skip the following instruction if the value of register VX is not equal to the value of register VY
void chip8_cpu_9XY0() {
    printf("SNE V%X, V%X", chip8.cpu.opcode.x, chip8.cpu.opcode.y);
}
