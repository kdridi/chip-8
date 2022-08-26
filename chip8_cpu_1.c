//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

// Jump to address NNN
void chip8_cpu_1NNN() {
    // printf("JP $%03X", chip8.cpu.opcode.nnn);
    // fflush(stdout);
    chip8.cpu.pc = chip8.cpu.opcode.nnn;
}
