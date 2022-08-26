//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

// Skip the following instruction if the value of register VX equals the value of register VY
void chip8_cpu_5XY0(){
    SKIP_NEXT_IF(VX == VY);
}
