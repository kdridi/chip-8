//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

// Jump to address NNN plus the value of register V0
void chip8_cpu_BNNN() {
    PC = NNN + V0;
}
