#include "chip8_gen.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <rom_file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f)
    {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    uint16_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t data[4096];
    fread(data, 1, size, f);
    fclose(f);

    chip8_rom_load(data, size);
    chip8_reset();
    while (1)
        chip8_step();
    return 0;
}
