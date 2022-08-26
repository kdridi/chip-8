//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

// Skip the following instruction if the value of register VX is not equal to NN
void chip8_cpu_4XNN() {
    SKIP_NEXT_IF(VX != NN);
}
