//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>

// Skip the following instruction if the value of register VX is not equal to NN
void chip8_cpu_4XNN() {
    printf("SNE V%X, $%02X", chip8.cpu.opcode.x, chip8.cpu.opcode.nn);
}
