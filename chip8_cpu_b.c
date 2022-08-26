//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>

// Jump to address NNN plus the value of register V0
void chip8_cpu_BNNN() {
    printf("JP V0, $%03X", chip8.cpu.opcode.nnn);
}
