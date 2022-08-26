//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

int chip8_rom_load(const char *filename) {
    chip8.rom_file.name = filename;

    FILE *rom = fopen(chip8.rom_file.name, "rb");
    if (!rom) {
        fprintf(stderr,"Failed to open rom file %s\n", chip8.rom_file.name);
        return EXIT_FAILURE;
    }

    fseek(rom, 0, SEEK_END);
    chip8.rom_file.length = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    fread(chip8.ram + 0x200, 1, chip8.rom_file.length, rom);
    fclose(rom);

    printf("ROM size: %ld\n", chip8.rom_file.length);

    return EXIT_SUCCESS;
}
