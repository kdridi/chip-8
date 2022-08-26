//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

// Jump to address NNN
void chip8_cpu_1NNN() {
    PC = NNN;
}
