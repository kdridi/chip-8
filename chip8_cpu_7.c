//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

// Add NN to register VX
void chip8_cpu_7XNN() {
    // printf("ADD V%X, $%02X", chip8.cpu.opcode.x, chip8.cpu.opcode.nn);
    // fflush(stdout);
    chip8.cpu.v[chip8.cpu.opcode.x] += chip8.cpu.opcode.nn;
}
