//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>

// Set register VX to NN
void chip8_cpu_6XNN() {
    printf("LD V%X, $%02X", chip8.cpu.opcode.x, chip8.cpu.opcode.nn);
}
