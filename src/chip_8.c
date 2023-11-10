#include "chip_8.h"
#include "definitions.h"
#include "display.h"
#include "display_buffer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

chip_8_t create_chip8(){
    chip_8_t chip;
    chip.delay_timer = 0;
    chip.Vf = &chip.registers[N_REG-1];
    chip.display = init_display(chip.display_buffer, BUFFER_SIZE);
    chip.pc = 0x200;
    return chip;
}

int load_chip8(chip_8_t* chip, const char* path){
    FILE* fp;
    uint8_t buffer[MEM_SIZE];
    fp = fopen(path, "rb");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), fp);
    if (bytes_read == 0) {
        printf("Error reading file\n");
        fclose(fp);
        return 1;
    }
    fclose(fp);
    for (int i = 0; i < bytes_read; i++) {
        chip->memory[0x200 + i] = buffer[i];
    }
    printf("Loaded %zu bytes into memory.\n", bytes_read);
    return 0;
}

instruction_t fetch(chip_8_t* chip_8){
    instruction_t inst;
    inst.instruction = ((uint16_t)chip_8->memory[chip_8->pc]) << 8;
    inst.instruction |= (uint16_t)chip_8->memory[chip_8->pc + 1];
    uint16_t mask = (1 << 4) - 1;
    inst.n0 = (inst.instruction & (mask << 12)) >> 12;
    inst.n1 = (inst.instruction & (mask << 8)) >> 8;
    inst.n2 = (inst.instruction & (mask << 4)) >> 4;
    inst.n3 = inst.instruction & mask;
    chip_8->pc += 2;
    return inst;
}

uint8_t get_kk(instruction_t *inst){
    return inst->n3 + 16 * inst->n2;
}

void destroy_chip8(chip_8_t* chip8){
    destroy_display(&chip8->display);
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
            return OpcodeCXNN;
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
            switch ((inst.n2 << 4) | inst.n3) {
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
                case 0x55:
                    return OpcodeFX55;
                    break;
                case 0x65:
                    return OpcodeFX65;
                    break;
            }
        break;
    }
    return Opcode0000;
}

void Opcode0NNN(chip_8_t* chip, instruction_t* inst){}

void Opcode00EE(chip_8_t* chip, instruction_t* inst){
    if(chip->sp > 0){
        chip->sp--;
        chip->pc = chip->stack[chip->sp];
    }

}

void Opcode00E0(chip_8_t* chip, instruction_t* inst){
    memset(chip->display.buffer, 0, BUFFER_LEN);
}

void Opcode1NNN(chip_8_t* chip, instruction_t* inst){ 
    uint16_t mem_addr = inst->instruction & ((1 << 12 ) - 1);
    chip->pc = mem_addr;
}

void Opcode2NNN(chip_8_t* chip, instruction_t* inst){ 
    uint16_t mem_addr = inst->instruction & ((1 << 11) - 1);
    chip->stack[chip->sp] = chip->pc;
    chip->sp++;
    chip->pc = mem_addr;
}

void Opcode3XKK(chip_8_t* chip, instruction_t* inst){ 
    uint8_t Vx = chip->registers[inst->n1];
    uint8_t kk = get_kk(inst);
    if(Vx == kk){
        chip->pc += 2;
    }
}

void Opcode4XKK(chip_8_t* chip, instruction_t* inst){ 
    uint8_t Vx = chip->registers[inst->n1];
    uint8_t kk = get_kk(inst);
    if(Vx != kk){
        chip->pc += 2;
    }
}

void Opcode5XY0(chip_8_t* chip, instruction_t* inst){ 
    uint8_t Vx = chip->registers[inst->n1];
    uint8_t Vy = chip->registers[inst->n2];
    if(Vx == Vy){
        chip->pc += 2;
    }
}

void Opcode6XKK(chip_8_t* chip, instruction_t* inst){ 
    uint8_t kk = get_kk(inst);
    chip->registers[inst->n1] = kk;
}

void Opcode7XKK(chip_8_t* chip, instruction_t* inst){
    uint8_t* Vx = &(chip->registers[inst->n1]);
    uint8_t kk = get_kk(inst);
    *Vx = *Vx + kk;
}

void Opcode8XY0(chip_8_t* chip, instruction_t* inst){
    uint8_t* Vx = &(chip->registers[inst->n1]);
    uint8_t* Vy = &(chip->registers[inst->n2]);
    *Vx = *Vy;
}

void Opcode8XY1(chip_8_t* chip, instruction_t* inst){
    uint8_t* Vx = &(chip->registers[inst->n1]);
    uint8_t* Vy = &(chip->registers[inst->n2]);
    *Vx = *Vy | *Vx;
}

void Opcode8XY2(chip_8_t* chip, instruction_t* inst){
    uint8_t* Vx = &(chip->registers[inst->n1]);
    uint8_t* Vy = &(chip->registers[inst->n2]);
    *Vx = *Vy & *Vx;
}


void Opcode8XY3(chip_8_t* chip, instruction_t* inst){
    uint8_t* Vx = &(chip->registers[inst->n1]);
    uint8_t* Vy = &(chip->registers[inst->n2]);
    *Vx = *Vy ^ *Vx;
}

void Opcode8XY4(chip_8_t* chip, instruction_t* inst){
    uint8_t* Vx = &(chip->registers[inst->n1]);
    uint8_t* Vy = &(chip->registers[inst->n2]);
    *chip->Vf = ((UINT8_MAX - *Vx) < *Vy);
    *Vx = *Vx + *Vy;
}

void Opcode8XY5(chip_8_t* chip, instruction_t* inst){
    uint8_t* Vx = &(chip->registers[inst->n1]);
    uint8_t* Vy = &(chip->registers[inst->n2]);
    *chip->Vf = (*Vx > *Vy);
    *Vx = *Vx - *Vy;
}

void Opcode8XY6(chip_8_t* chip, instruction_t* inst){
    uint8_t* Vx = &(chip->registers[inst->n1]);
    *chip->Vf = *Vx & 0x01;
    *Vx >>= 1;
}

void Opcode8XY7(chip_8_t* chip, instruction_t* inst){
    uint8_t* Vx = &(chip->registers[inst->n1]);
    uint8_t* Vy = &(chip->registers[inst->n2]);
    *chip->Vf = (*Vy > *Vx);
    *Vx = *Vy - *Vx;
}

void Opcode8XYE(chip_8_t* chip, instruction_t* inst){
    uint8_t* Vx = &(chip->registers[inst->n1]);
    *chip->Vf = *Vx >> 7;
    *Vx <<= 1;
}

void Opcode9XY0(chip_8_t* chip, instruction_t* inst){
    uint8_t* Vx = &(chip->registers[inst->n1]);
    uint8_t* Vy = &(chip->registers[inst->n2]);
    if(*Vx != *Vy){
        chip->pc += 2;
    }
}

void OpcodeANNN(chip_8_t* chip, instruction_t* inst){
    uint16_t val = inst->instruction & ((1 << 11) - 1);
    chip->I = val;
}

void OpcodeBNNN(chip_8_t* chip, instruction_t* inst){
    uint16_t mem_addr = inst->instruction & ((1 << 11) - 1);
    chip->pc = chip->registers[0] + mem_addr;
}

void OpcodeCXNN(chip_8_t* chip, instruction_t* inst){
    uint8_t val = inst->instruction & ((1 << 7) - 1);
    chip->registers[inst->n1] = val & random_number();
}

void OpcodeDXYN(chip_8_t* chip, instruction_t* inst){
    uint16_t sprite_addr = chip->I;
    uint8_t sprite;
    uint8_t x = chip->registers[inst->n1];
    uint8_t y_start = chip->registers[inst->n2];
    *chip->Vf = 0;
    uint8_t row;
    for(uint8_t i = 0; i < inst->n3; i++){
        sprite=chip->memory[sprite_addr + i];
        row = y_start + i;
        uint8_t existing = get_byte_row_col(chip->display_buffer, row, x);
        uint8_t xored = sprite ^= existing;
        *chip->Vf |= (xored & existing) != 0;
        set_byte_row_col(chip->display_buffer, row, x, xored);
    }
}

void OpcodeEX9E(chip_8_t* chip, instruction_t* inst){
    if(get_key(&chip->display, chip->registers[inst->n1])){
        chip->pc += 2;
    }
}

void OpcodeEXA1(chip_8_t* chip, instruction_t* inst){
    if(!get_key(&chip->display, chip->registers[inst->n1])){
        chip->pc += 2;
    }
}

void OpcodeFX07(chip_8_t* chip, instruction_t* inst){
    chip->registers[inst->n1] = chip->delay_timer;
}

void OpcodeFX0A(chip_8_t* chip, instruction_t* inst){
    if(!chip->display.keypad){
        chip->pc -= 2;
    } else {
        chip->registers[inst->n1] = ffs(chip->display.keypad) - 1;
    }
}

void OpcodeFX15(chip_8_t* chip, instruction_t* inst){
    chip->delay_timer = chip->registers[inst->n1];
}

void OpcodeFX18(chip_8_t* chip, instruction_t* inst){
    chip->sound_timer = chip->registers[inst->n1];
}

void OpcodeFX1E(chip_8_t* chip, instruction_t* inst){
    chip->I += chip->registers[inst->n1];
}

void OpcodeFX29(chip_8_t* chip, instruction_t* inst){
    chip->I = FONT_START + (5 * chip->registers[inst->n1]);
}

void OpcodeFX33(chip_8_t* chip, instruction_t* inst){
    uint8_t vx = chip->registers[inst->n1];
    chip->memory[chip->I] = (vx % 1000) / 100;
    chip->memory[chip->I + 1] = (vx % 100) / 10;
    chip->memory[chip->I + 2] = (vx % 10);
}

void OpcodeFX55(chip_8_t* chip, instruction_t* inst){
    memcpy(&(chip->memory[chip->I]), chip->registers, inst->n1);
}


void OpcodeFX65(chip_8_t* chip, instruction_t* inst){
    memcpy(chip->registers, &chip->memory[chip->I], inst->n1);
}

void Opcode0000(chip_8_t* chip, instruction_t* inst){
    printf("FUCK YOU");
}

void init_random(){
    srand(time(0));
}

uint8_t random_number(){
    static int random_init = 0;
    if(!random_init){
        init_random();
        random_init = 1;
    }
    static const uint8_t upper = 255;
    static const uint8_t  lower = 0;
    return (rand() % (upper - lower + 1)) + lower;
}
