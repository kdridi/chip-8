//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

static opcode_handler_t handlers[0x100];

void chip8_cpu_E___() {
    opcode_handler_t h = handlers[chip8.cpu.opcode.nn];
    if (h) {
        h();
    } else {
        fprintf(stderr, "Unknown opcode: 0x%04x\n", chip8.cpu.opcode);
        abort();
    }
}

// Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
static void chip8_cpu_EX9E() {
    printf("SKP V%X", chip8.cpu.opcode.x);
}

// Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
static void chip8_cpu_EXA1() {
    printf("SKNP V%X", chip8.cpu.opcode.x);
}

__attribute__((constructor))
static void chip8_cpu_E___init() {
    memset(handlers, 0, sizeof(handlers));
    handlers[0x9e] = chip8_cpu_EX9E;
    handlers[0xa1] = chip8_cpu_EXA1;
}
