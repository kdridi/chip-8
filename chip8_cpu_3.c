//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>

// Skip next instruction if the value of register VX equals NN
void chip8_cpu_3XNN() {
    printf("SE V%X, $%02X", chip8.cpu.opcode.x, chip8.cpu.opcode.nn);
}
