//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

// Store memory address NNN in register I
void chip8_cpu_ANNN() {
    I = NNN;
}
