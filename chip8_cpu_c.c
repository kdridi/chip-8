//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

// Set register VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
void chip8_cpu_CXNN() {
    VX = (rand() % 0x100) & NN;
}
