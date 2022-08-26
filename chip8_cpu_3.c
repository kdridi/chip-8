//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

// Skip next instruction if the value of register VX equals NN
void chip8_cpu_3XNN() {
    SKIP_NEXT_IF(VX == NN);
}
