//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

static opcode_handler_t handlers[0x100];

void chip8_cpu_0___(void) {
    opcode_handler_t handler = handlers[chip8.cpu.opcode.nn];
    if (handler) {
        handler();
    } else {
        fprintf(stderr, "Unknown opcode::: 0x%04x\n", chip8.cpu.opcode);
        abort();
    }
}

// Execute machine language subroutine at address NNN
static void chip8_cpu_0NNN() {
    printf("SYS $%03X\n", chip8.cpu.opcode.nnn);
    fflush(stdout);
    abort();
    chip8.cpu.stack[chip8.cpu.sp++] = chip8.cpu.pc;
    chip8.cpu.pc = chip8.cpu.opcode.nnn;
}

// Clear the screen
static void chip8_cpu_00E0() {
    // printf("CLS");
    // fflush(stdout);
    for (int i = 0, k = 0; i < SCREEN_H; ++i) {
        for (int j = 0; j < SCREEN_W; ++j, ++k) {
            chip8.screen[k] = 0;
        }
    }
    chip8.screen_redraw = true;
}

// Return from a subroutine
static void chip8_cpu_00EE() {
    // printf("RET");
    // fflush(stdout);
    chip8.cpu.pc = chip8.cpu.stack[--chip8.cpu.sp];
}

__attribute__((constructor))
static void chip8_cpu_0___init() {
    size_t len = sizeof(handlers) / sizeof(*handlers);
    for (size_t i = 0; i < len; ++i)
        handlers[i] = chip8_cpu_0NNN;
    handlers[0xe0] = chip8_cpu_00E0;
    handlers[0xee] = chip8_cpu_00EE;
}
