//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

// Execute subroutine at address NNN
void chip8_cpu_2NNN() {
    // printf("CALL $%03X", chip8.cpu.opcode.nnn);
    // fflush(stdout);
    chip8.cpu.stack[chip8.cpu.sp++] = chip8.cpu.pc;
    chip8.cpu.pc = chip8.cpu.opcode.nnn;
}
