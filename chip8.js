const lines = [];
lines.push(`#include <memory.h>`);
lines.push(`#include <stdlib.h>`);
lines.push(`#include <stdint.h>`);
lines.push(`#include <stdio.h>`);
lines.push(`#include <stdbool.h>`);
lines.push(``);
lines.push(`static uint16_t pc = 0x200;`);
lines.push(`static uint16_t i = 0;`);
lines.push(`static uint16_t sp = 0;`);
lines.push(`static uint16_t stack[16];`);
lines.push(`static uint8_t memory[4096];`);
lines.push(`static uint8_t v[16];`);
lines.push(`static bool vram[64 * 32];`);
lines.push(`static bool dirty = true;`);
lines.push(``);

new Array(0x1000).fill(0).forEach(function(_, i) {
    lines.push(`static void op_0${i.toString(16).padStart(3, '0')}(void) {`);
    switch (i) {
        case 0x00E0:
            lines.push(`    // CLS`);
            lines.push(`    memset(vram, 0, sizeof(vram));`);
            lines.push(`    dirty = true;`);
            break;
        case 0x00EE:
            lines.push(`    // RET`);
            lines.push(`    pc = stack[--sp];`);
            break;
        default:
            lines.push(`    // SYS ${i.toString(16).padStart(3, '0')}`);
            lines.push(`    abort();`);
            break;
    }
    lines.push(`}`);
    lines.push(``);
})

new Array(0x1000).fill(0).forEach(function(_, i) {
    lines.push(`static void op_1${i.toString(16).padStart(3, '0')}(void) {`);
    lines.push(`    // JP $${i.toString(16).padStart(4, '0')}`);
    lines.push(`    pc = 0x${i.toString(16).padStart(4, '0')};`);
    lines.push(`}`);
    lines.push(``);
})

new Array(0x1000).fill(0).forEach(function(_, i) {
    lines.push(`static void op_2${i.toString(16).padStart(3, '0')}(void) {`);
    lines.push(`    // CALL $${i.toString(16).padStart(4, '0')}`);
    lines.push(`    stack[sp++] = pc;`);
    lines.push(`    pc = 0x${i.toString(16).padStart(4, '0')};`);
    lines.push(`}`);
    lines.push(``);
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_3${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // SE V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    if (v[0x${x.toString(16).padStart(1, '0')}] == 0x${nn.toString(16).padStart(3, '0')}) {`);
        lines.push(`        pc += 2;`);
        lines.push(`    }`);
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_4${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // SNE V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    if (v[0x${x.toString(16).padStart(1, '0')}] != 0x${nn.toString(16).padStart(3, '0')}) {`);
        lines.push(`        pc += 2;`);
        lines.push(`    }`);
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_5${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // SE V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    if (v[0x${x.toString(16).padStart(1, '0')}] == 0x${nn.toString(16).padStart(3, '0')}) {`);
        lines.push(`        pc += 2;`);
        lines.push(`    }`);
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_6${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // LD V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = 0x${nn.toString(16).padStart(3, '0')};`);
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_7${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // ADD V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] += 0x${nn.toString(16).padStart(3, '0')};`);
        lines.push(`}`);
        lines.push(``);
    })
})


new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x10).fill(0).forEach(function(_, y) {
        new Array(0x10).fill(0).forEach(function(_, n) {
            lines.push(`static void op_8${x.toString(16).padStart(1, '0')}${y.toString(16).padStart(1, '0')}${n.toString(16).padStart(1, '0')}(void) {`);
            switch (n) {
                case 0x0:
                    lines.push(`    // LD V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = v[0x${y.toString(16).padStart(1, '0')}];`);
                    break;
                case 0x1:
                    lines.push(`    // OR V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] |= v[0x${y.toString(16).padStart(1, '0')}];`);
                    break;
                case 0x2:
                    lines.push(`    // AND V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] &= v[0x${y.toString(16).padStart(1, '0')}];`);
                    break;
                case 0x3:
                    lines.push(`    // XOR V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] ^= v[0x${y.toString(16).padStart(1, '0')}];`);
                    break;
                case 0x4:
                    lines.push(`    // ADDC V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    int16_t result = v[0x${x.toString(16).padStart(1, '0')}] + v[0x${y.toString(16).padStart(1, '0')}];`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = result & 0x0ff;`);
                    lines.push(`    v[0xf] = result > 0x0ff ? 1 : 0;`);
                    break;
                case 0x5:
                    lines.push(`    // SUBC V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    int16_t result = v[0x${x.toString(16).padStart(1, '0')}] - v[0x${y.toString(16).padStart(1, '0')}];`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = result & 0x0ff;`);
                    lines.push(`    v[0xf] = result < 0x000 ? 0 : 1;`);
                    break;
                case 0x6:
                    lines.push(`    // SHR V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = v[0x${y.toString(16).padStart(1, '0')}] >> 1;`);
                    lines.push(`    v[0xf] = v[0x${y.toString(16).padStart(1, '0')}] & 0x01;`);
                    break;
                case 0x7:
                    lines.push(`    // SUBNC V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    int16_t result = v[0x${y.toString(16).padStart(1, '0')}] - v[0x${x.toString(16).padStart(1, '0')}];`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = result & 0x0ff;`);
                    lines.push(`    v[0xf] = result < 0x000 ? 0 : 1;`);
                    break;
                case 0xe:
                    lines.push(`    // SHL V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = v[0x${y.toString(16).padStart(1, '0')}] << 1;`);
                    lines.push(`    v[0xf] = v[0x${y.toString(16).padStart(1, '0')}] & 0x80 ? 1 : 0;`);
                    break;
                default:
                    lines.push(`    // UNKNOWN`);
                    lines.push(`    fprintf(stderr, "Unknown opcode: 0x8${x.toString(16).padStart(1, '0')}${y.toString(16).padStart(1, '0')}${n.toString(16).padStart(1, '0')}\\n");`);
                    lines.push(`    abort();`);
                    break;
            }
            lines.push(`}`);
            lines.push(``);
        })
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_9${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // SE V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    if (v[0x${x.toString(16).padStart(1, '0')}] != 0x${nn.toString(16).padStart(3, '0')}) {`);
        lines.push(`        pc += 2;`);
        lines.push(`    }`);
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x1000).fill(0).forEach(function(_, nnn) {
    lines.push(`static void op_a${nnn.toString(16).padStart(3, '0')}(void) {`);
    lines.push(`    // LD I, $${nnn.toString(16).padStart(3, '0')}`);
    lines.push(`    i = 0x${nnn.toString(16).padStart(3, '0')};`);
    lines.push(`}`);
    lines.push(``);
})

// lines.push(`typedef void (*opcode_fn)(void);`);
// lines.push(`opcode_fn opcode_fns[] = {`);
// new Array(0x06).fill(0).forEach(function(_, x) {
//     new Array(0x1000).fill(0).forEach(function(_, nnn) {
//         lines.push(`    op_${x.toString(16).padStart(1, '0')}${nnn.toString(16).padStart(3, '0')},`);
//     })
// })
// lines.push(`};`);
// lines.push(``);




console.log(lines.join('\n'));
