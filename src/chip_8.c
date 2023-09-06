#include "chip_8.h"
#include <stdint.h>
#include <stdio.h>

instruction_t fetch(chip_8_t* chip_8){
    instruction_t inst;
    inst.instruction = ((uint16_t)chip_8->memory[chip_8->pc] << 8) |
                          chip_8->memory[chip_8->pc + 1];
    uint16_t mask = (1 << 4) - 1;
    inst.n0 = inst.instruction & (mask << 12);
    inst.n1 = inst.instruction & (mask << 8);
    inst.n2 = inst.instruction & (mask << 4);
    inst.n3 = inst.instruction & mask;
    chip_8->pc += 2;
    return inst;
}

op decode_instruction(instruction_t inst){
    switch (inst.n0) {
        case 0x0:
            switch (inst.n3) {
                case 0xE:
                    return Opcode00EE;
                    break;
                case 0x0:
                    return Opcode00E0;
                    break;
            }
        case 0x1:
            return Opcode1NNN;
            break;
        case 0x2:
            return Opcode2NNN;
            break;
        case 0x3:
            return Opcode3XKK;
            break;
        case 0x4:
            return Opcode4XKK;
            break;
        case 0x5:
            return Opcode5XY0;
            break;
        case 0x6:
            return Opcode6XKK;
            break;
        case 0x7:
            return Opcode7XKK;
            break;
        case 0x8:
            switch (inst.n3) {
                case 0x0:
                    return Opcode8XY0;
                    break;
                case 0x1:
                    return Opcode8XY1;
                    break;
                case 0x2:
                    return Opcode8XY2;
                    break;
                case 0x3:
                    return Opcode8XY3;
                    break;
                case 0x4:
                    return Opcode8XY4;
                    break;
                case 0x5:
                    return Opcode8XY5;
                    break;
                case 0x6:
                    return Opcode8XY6;
                    break;
                case 0x7:
                    return Opcode8XY7;
                    break;
                case 0xE:
                    return Opcode8XYE;
                    break;
            }
            break;
        case 0x9:
            return Opcode9XY0;
            break;
        case 0xA:
            return OpcodeANNN;
            break;
        case 0xB:
            return OpcodeBNNN;
            break;
        case 0xC:
            return OpcodeCXKK;
            break;
        case 0xD:
            return OpcodeDXYN;
            break;
        case 0xE:
            switch (inst.n3) {
                case 0xE:
                    return OpcodeEX9E;
                    break;
                case 0x1:
                    return OpcodeEXA1;
                    break;
            }
        case 0xF:
            switch (inst.n2 | inst.n3) {
                case 0x07:
                    return OpcodeFX07;
                    break;
                case 0x0A:
                    return OpcodeFX0A;
                    break;
                case 0x15:
                    return OpcodeFX15;
                    break;
                case 0x18:
                    return OpcodeFX18;
                    break;
                case 0x1E:
                    return OpcodeFX1E;
                    break;
                case 0x29:
                    return OpcodeFX29;
                    break;
                case 0x33:
                    return OpcodeFX33;
                    break;
                case 0x65:
                    return OpcodeFX65;
                    break;
            }
        break;
    }
    return Opcode0000;
}
