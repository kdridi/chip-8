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
    printf("SYS $%03X\n", NNN);
    fflush(stdout);
    abort();
}

// Clear the screen
static void chip8_cpu_00E0() {
    memset(chip8.screen, 0, sizeof(chip8.screen));
    chip8.screen_redraw = true;
}

// Return from a subroutine
static void chip8_cpu_00EE() {
    PC = STACK_POP();
}

__attribute__((constructor))
static void chip8_cpu_0___init() {
    size_t len = sizeof(handlers) / sizeof(*handlers);
    for (size_t i = 0; i < len; ++i)
        handlers[i] = chip8_cpu_0NNN;
    handlers[0xe0] = chip8_cpu_00E0;
    handlers[0xee] = chip8_cpu_00EE;
}
