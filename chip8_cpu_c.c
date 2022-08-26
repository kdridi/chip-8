//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>

// Set register VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
void chip8_cpu_CXNN() {
    printf("RND V%X, $%02X", chip8.cpu.opcode.x, chip8.cpu.opcode.nn);
}
