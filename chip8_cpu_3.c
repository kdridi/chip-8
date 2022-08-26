//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

// Skip next instruction if the value of register VX equals NN
void chip8_cpu_3XNN() {
    // printf("SE V%X, $%02X", chip8.cpu.opcode.x, chip8.cpu.opcode.nn);
    // fflush(stdout);
    if (chip8.cpu.v[chip8.cpu.opcode.x] == chip8.cpu.opcode.nn) {
        chip8.cpu.pc += 2;
    }
}
