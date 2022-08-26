//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>

opcode_handler_t handlers[] = {
        chip8_cpu_0___,
        chip8_cpu_1NNN,
        chip8_cpu_2NNN,
        chip8_cpu_3XNN,
        chip8_cpu_4XNN,
        chip8_cpu_5XY0,
        chip8_cpu_6XNN,
        chip8_cpu_7XNN,
        chip8_cpu_8___,
        chip8_cpu_9XY0,
        chip8_cpu_ANNN,
        chip8_cpu_BNNN,
        chip8_cpu_CXNN,
        chip8_cpu_DXYN,
        chip8_cpu_E___,
        chip8_cpu_F___,
};

void chip8_disassemble() {
    typeof(chip8.cpu.pc) pc = chip8.cpu.pc;
    chip8.cpu.pc = 0x200;
    while (chip8.cpu.pc < chip8.rom_file.length + 0x200) {
        chip8.cpu.opcode.value = 0;
        printf("%04lx: ", chip8.cpu.pc);

        chip8.cpu.opcode.l = chip8.ram[chip8.cpu.pc++];
        chip8.cpu.opcode.h = chip8.ram[chip8.cpu.pc++];
        printf("%04x: ", chip8.cpu.opcode.value);

        handlers[chip8.cpu.opcode.f]();

        printf("\n");
    }
    chip8.cpu.pc = pc;
}
