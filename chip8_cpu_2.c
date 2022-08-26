//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>

// Execute subroutine at address NNN
void chip8_cpu_2NNN() {
    printf("CALL $%03X", chip8.cpu.opcode.nnn);
}
