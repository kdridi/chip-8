//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>

// Draw a sprite at coordinate (VX, VY) with N bytes of sprite data starting at the address stored in register I
void chip8_cpu_DXYN() {
    printf("DRW V%X, V%X, $%02X", chip8.cpu.opcode.x, chip8.cpu.opcode.y, chip8.cpu.opcode.n);
}
