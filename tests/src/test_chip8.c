#include "test_chip8.h"
#include "display.h"
#include "display_buffer.h"
#include "test_macro.h"
#include <stdio.h>
#include "chip_8.h"
#include "definitions.h"
#include "assert.h"


chip_8_t get_test_chip(uint16_t instruction){
    chip_8_t result = create_chip8();
    memset(result.memory, 0, MEM_SIZE);
    memset(result.registers, 0, N_REG);
    result.memory[TEST_INSTRUCTION_LOCATION] = (uint8_t)(instruction >> 8);
    result.memory[TEST_INSTRUCTION_LOCATION + 1] = (uint8_t)(instruction);
    result.pc =  TEST_INSTRUCTION_LOCATION;
    return result;
}

void test_Opcode00EE(){
    chip_8_t chip = get_test_chip(0x2004);
    chip.memory[TEST_INSTRUCTION_LOCATION + 4] = 0x00;
    chip.memory[TEST_INSTRUCTION_LOCATION + 5] = 0xEE;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst); // RET instruction
    assert(chip.pc == (TEST_INSTRUCTION_LOCATION + 2));
    destroy_chip8(&chip);
}

void test_Opcode00E0(){
    chip_8_t chip = get_test_chip(0x00E0);
    instruction_t inst = fetch(&chip);
    memset(chip.display_buffer, 255, 10);
    decode_instruction(inst)(&chip, &inst);
    for (size_t i = 0; i < BUFFER_SIZE; i++) {
        assert(chip.display.buffer[i] == 0);
    }
    destroy_chip8(&chip);
}

void test_Opcode1NNN(){
    chip_8_t chip = get_test_chip(0x1011);
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == 0x011);
    destroy_chip8(&chip);
}

void test_Opcode2NNN(){
    chip_8_t chip = get_test_chip(0x2511);
    instruction_t inst = fetch(&chip);
    uint8_t sp = chip.sp;
    uint8_t pc = chip.pc;
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == 0x511);
    assert(chip.sp = sp + 1);
    assert(chip.stack[sp] == pc);
    destroy_chip8(&chip);
}
void test_Opcode3XKK(){
    chip_8_t chip = get_test_chip(0x300A);
    uint8_t pc = chip.pc;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == pc + 2);
    chip = get_test_chip(0x300A);
    pc = chip.pc;
    chip.registers[0] = 0xA;
    inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == pc+4);
    destroy_chip8(&chip);
}

void test_Opcode4XKK(){
    chip_8_t chip = get_test_chip(0x400A);
    uint8_t pc = chip.pc;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == pc + 4);
    chip = get_test_chip(0x400A);
    pc = chip.pc;
    chip.registers[0] = 0xA;
    inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == pc+2);
    destroy_chip8(&chip);
}

void test_Opcode5XY0(){
    chip_8_t chip = get_test_chip(0x5120);
    chip.registers[1] = 2;
    chip.registers[2] = 3;
    uint8_t pc = chip.pc;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == pc+2);
    chip = get_test_chip(0x5120);
    chip.registers[1] = 2;
    chip.registers[2] = 2;
    pc = chip.pc;
    inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == pc+4);
    destroy_chip8(&chip);
}

void test_Opcode6XKK(){
    chip_8_t chip = get_test_chip(0x610A);
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[1] == 0xA);
    destroy_chip8(&chip);
};

void test_Opcode7XKK(){
    chip_8_t chip = get_test_chip(0x7201);
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[2] == 1);
    destroy_chip8(&chip);
}


void test_Opcode8XY0(){
    chip_8_t chip = get_test_chip(0x8560);
    chip.registers[6] = 10;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 10);
    destroy_chip8(&chip);
}

void test_Opcode8XY1(){
    chip_8_t chip = get_test_chip(0x8561);
    chip.registers[6] = 10;
    chip.registers[5] = 4;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 14);
    destroy_chip8(&chip);
}

void test_Opcode8XY2(){
    chip_8_t chip = get_test_chip(0x8562);
    chip.registers[6] = 10;
    chip.registers[5] = 7;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 2);
    destroy_chip8(&chip);
}


void test_Opcode8XY3(){
    chip_8_t chip = get_test_chip(0x8563);
    chip.registers[6] = 10;
    chip.registers[5] = 7;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 13);
    destroy_chip8(&chip);
}

void test_Opcode8XY4_no_carry(){
    chip_8_t chip = get_test_chip(0x8564);
    chip.registers[6] = 250;
    chip.registers[5] = 1;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 251);
    assert(*chip.Vf == 0);
    destroy_chip8(&chip);
}


void test_Opcode8XY4_carry(){
    chip_8_t chip = get_test_chip(0x8564);
    chip.registers[6] = 130;
    chip.registers[5] = 130;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 4);
    assert(*chip.Vf == 1);
    destroy_chip8(&chip);
}


void test_Opcode8XY5_no_borrow(){
    chip_8_t chip = get_test_chip(0x8565);
    chip.registers[6] = 9;
    chip.registers[5] = 10;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 1);
    assert(*chip.Vf == 1);
    destroy_chip8(&chip);
}


void test_Opcode8XY5_borrow(){
    chip_8_t chip = get_test_chip(0x8565);
    chip.registers[6] = 200;
    chip.registers[5] = 10;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 66);
    assert(*chip.Vf == 0);
    destroy_chip8(&chip);
}


void test_Opcode8XY6_flag(){
    chip_8_t chip = get_test_chip(0x8566);
    chip.registers[5] = 11;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 5);
    assert(*chip.Vf == 1);
    destroy_chip8(&chip);
}

void test_Opcode8XY6_no_flag(){
    chip_8_t chip = get_test_chip(0x8566);
    chip.registers[5] = 10;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 5);
    assert(*chip.Vf == 0);
    destroy_chip8(&chip);
}

void test_Opcode8XY7_borrow(){
    chip_8_t chip = get_test_chip(0x8567);
    chip.registers[6] = 10;
    chip.registers[5] = 9;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 1);
    assert(*chip.Vf == 1);
    destroy_chip8(&chip);
}

void test_Opcode8XY7_no_borrow(){
    chip_8_t chip = get_test_chip(0x8567);
    chip.registers[6] = 10;
    chip.registers[5] = 200;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 66);
    assert(*chip.Vf == 0);
    destroy_chip8(&chip);
}


void test_Opcode8XYE_flag(){
    chip_8_t chip = get_test_chip(0x856E);
    chip.registers[5] = 129;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 2);
    assert(*chip.Vf == 1);
    destroy_chip8(&chip);
}

void test_Opcode8XYE_no_flag(){
    chip_8_t chip = get_test_chip(0x856E);
    chip.registers[5] = 10;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[5] == 20);
    assert(*chip.Vf == 0);
    destroy_chip8(&chip);
}

void test_Opcode9XY0_eq(){
    chip_8_t chip = get_test_chip(0x9560);
    uint pc = chip.pc;
    chip.registers[6] = 10;
    chip.registers[5] = 10;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == pc + 2);
    destroy_chip8(&chip);
}

void test_Opcode9XY0_neq(){
    chip_8_t chip = get_test_chip(0x9560);
    uint pc = chip.pc;
    chip.registers[6] = 10;
    chip.registers[5] = 7;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == pc + 4);
    destroy_chip8(&chip);
}

void test_OpcodeANNN(){
    chip_8_t chip = get_test_chip(0xA100);
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.I == 0x100);
    destroy_chip8(&chip);
}

void test_OpcodeBNNN(){
    chip_8_t chip = get_test_chip(0xB010);
    chip.registers[0] = 10; 
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == 10 + 0x010);
    destroy_chip8(&chip);
}


void test_OpcodeDXYN(){
    {
        chip_8_t chip = get_test_chip(0xD458);
        uint16_t sprite_address = 0x20F;
        uint8_t x = 10;
        uint8_t y = 3;
        chip.registers[4] = x;
        chip.registers[5] = y;
        uint8_t spriteData[8] = {
            0b10000001,
            0b01000010,
            0b00100100,
            0b00011000,
            0b00011000,
            0b00100100,
            0b01000010,
            0b10000001
        };
        for (int i = 0; i < 8; i++) {
            chip.memory[sprite_address + i] = spriteData[i];
        }
        chip.I = 0x20F;
        instruction_t inst = fetch(&chip);
        decode_instruction(inst)(&chip, &inst);
        for (int i = 0; i < 8; i++) {
            assert(get_byte_row_col(chip.display.buffer, y + i, x) == spriteData[i]);
        }
        destroy_chip8(&chip);
    }
    {
        chip_8_t chip = get_test_chip(0xD458);
        uint16_t sprite_address = 0x20F;
        uint8_t x = 60;
        uint8_t y = 3;
        chip.registers[4] = x;
        chip.registers[5] = y;
        uint8_t spriteData[8] = {
            0b10000001,
            0b01000010,
            0b00100100,
            0b00011000,
            0b00011000,
            0b00100100,
            0b01000010,
            0b10000001
        };
        for (int i = 0; i < 8; i++) {
            chip.memory[sprite_address + i] = spriteData[i];
        }
        chip.I = 0x20F;
        instruction_t inst = fetch(&chip);
        decode_instruction(inst)(&chip, &inst);
        for (int i = 0; i < 8; i++) {
            assert(get_byte_row_col(chip.display.buffer, y + i, x) == spriteData[i]);
        }
        destroy_chip8(&chip);
    }
}

void test_OpcodeEX9E(){
    {
        chip_8_t chip = get_test_chip(0xE29E);
        chip.registers[2] = 4;
        set_key(&chip.display,4 , 1);
        uint8_t pc = chip.pc;
        instruction_t inst = fetch(&chip);
        decode_instruction(inst)(&chip, &inst);
        assert(chip.pc == pc + 4);
        destroy_chip8(&chip);
    }

    {
        chip_8_t chip = get_test_chip(0xE29E);
        chip.registers[2] = 4;
        set_key(&chip.display,4 , 0);
        uint8_t pc = chip.pc;
        instruction_t inst = fetch(&chip);
        decode_instruction(inst)(&chip, &inst);
        assert(chip.pc == pc + 2);
        destroy_chip8(&chip);
    }
}

void test_OpcodeEXA1(){
    {
        chip_8_t chip = get_test_chip(0xE2A1);
        chip.registers[2] = 4;
        set_key(&chip.display,4 , 1);
        uint8_t pc = chip.pc;
        instruction_t inst = fetch(&chip);
        decode_instruction(inst)(&chip, &inst);
        assert(chip.pc == pc + 2);
        destroy_chip8(&chip);
    }
    {
        chip_8_t chip = get_test_chip(0xE2A1);
        chip.registers[2] = 4;
        set_key(&chip.display,4 , 0);
        uint8_t pc = chip.pc;
        instruction_t inst = fetch(&chip);
        decode_instruction(inst)(&chip, &inst);
        assert(chip.pc == pc + 4);
        destroy_chip8(&chip);
    }
}

void test_OpcodeFX07(){
    chip_8_t chip = get_test_chip(0xF107);
    chip.delay_timer = 2;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.registers[1] == 2);
    destroy_chip8(&chip);
}

void test_OpcodeFX0A(){
    chip_8_t chip = get_test_chip(0xF10A);
    chip.delay_timer = 2;
    chip.registers[1] = 2;
    uint8_t pc = chip.pc;
    set_key(&chip.display, 2, 0);
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc == pc);
    set_key(&chip.display, 2, 1);
    inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.pc = pc + 2);
    destroy_chip8(&chip);
}

void test_OpcodeFX15(){
    chip_8_t chip = get_test_chip(0xF115);
    chip.registers[1] = 50;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    destroy_chip8(&chip);
    assert(chip.delay_timer == 50);
}

void test_OpcodeFX18(){
    chip_8_t chip = get_test_chip(0xF118);
    chip.registers[1] = 50;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.sound_timer == 50);
    destroy_chip8(&chip);
}

void test_OpcodeFX1E(){
    chip_8_t chip = get_test_chip(0xF11E);
    chip.registers[1] = 50;
    chip.I = 10;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.I == 60);
    destroy_chip8(&chip);
}

void test_OpcodeFX29(){
    chip_8_t chip = get_test_chip(0xF129);
    chip.registers[1] = 3;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.I == FONT_START + (5 * 3));
    destroy_chip8(&chip);
}

void test_OpcodeFX33(){
    chip_8_t chip = get_test_chip(0xF133);
    chip.registers[1] = 253;
    chip.I = 0x500;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    assert(chip.memory[chip.I] == 2);
    assert(chip.memory[chip.I + 1] == 5);
    assert(chip.memory[chip.I + 2] == 3);
    destroy_chip8(&chip);
}

//Store registers V0 through Vx in memory starting at location I.
//The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
void test_OpcodeFX55(){
    chip_8_t chip = get_test_chip(0xF655);
    uint8_t random_numbers[6];
    for(int i = 0; i < 6; i++){
        random_numbers[i] = random_number();
        chip.registers[i] = random_numbers[i];
    }
    chip.I = 0x500;
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    for(int i = 0; i < 6; i++){
        assert(chip.memory[chip.I + i] == random_numbers[i]);
    }
    destroy_chip8(&chip);
}


void test_OpcodeFX65(){
    chip_8_t chip = get_test_chip(0xF665);
    uint8_t random_numbers[6];
    chip.I = 0x500;
    for(int i = 0; i < 6; i++){
        random_numbers[i] = random_number();
        chip.memory[chip.I + i] = random_numbers[i];
    }
    instruction_t inst = fetch(&chip);
    decode_instruction(inst)(&chip, &inst);
    for(int i = 0; i < 6; i++){
        assert(chip.registers[i] == random_numbers[i]);
    }
    destroy_chip8(&chip);
}
int main(){
    TEST("Opcode00E0", test_Opcode00E0);
    TEST("Opcode00EE", test_Opcode00EE);
    TEST("Opcode1NNN", test_Opcode1NNN);
    TEST("Opcode2NNN", test_Opcode2NNN);
    TEST("Opcode3XKK", test_Opcode3XKK);
    TEST("Opcode4XKK", test_Opcode4XKK);
    TEST("Opcode5XY0", test_Opcode5XY0);
    TEST("Opcode6XKK", test_Opcode6XKK);
    TEST("Opcode7XKK", test_Opcode7XKK);
    TEST("Opcode8XY0", test_Opcode8XY0);
    TEST("Opcode8XY1", test_Opcode8XY1);
    TEST("Opcode8XY2", test_Opcode8XY2);
    TEST("Opcode8XY3", test_Opcode8XY3);
    TEST("Opcode8XY4_no_carry", test_Opcode8XY4_no_carry);
    TEST("Opcode8XY4_carry", test_Opcode8XY4_carry);
    TEST("Opcode8XY5_no_borrow", test_Opcode8XY5_no_borrow);
    TEST("Opcode8XY5_borrow", test_Opcode8XY5_borrow);
    TEST("Opcode8XY6_flag", test_Opcode8XY6_flag);
    TEST("Opcode8XY6_no_flag", test_Opcode8XY6_no_flag);
    TEST("Opcode8XY7_no_borrow", test_Opcode8XY7_no_borrow);
    TEST("Opcode8XY7_borrow", test_Opcode8XY7_borrow);
    TEST("Opcode8XYE_flag", test_Opcode8XYE_flag);
    TEST("Opcode8XYE_no_flag", test_Opcode8XYE_no_flag);
    TEST("Opcode9XY0_eq", test_Opcode9XY0_eq);
    TEST("Opcode9XY0_neq", test_Opcode9XY0_neq);
    TEST("OpcodeANNN", test_OpcodeANNN);
    TEST("OpcodeBNNN", test_OpcodeBNNN);
    TEST("OpcodeDXYN", test_OpcodeDXYN);
    TEST("OpcodeEX9E", test_OpcodeEX9E);
    TEST("OpcodeEXA1", test_OpcodeEXA1);
    TEST("OpcodeFX07", test_OpcodeFX07);
    TEST("OpcodeFX0A", test_OpcodeFX0A);
    TEST("OpcodeFX15", test_OpcodeFX15);
    TEST("OpcodeFX18", test_OpcodeFX18);
    TEST("OpcodeFX1E", test_OpcodeFX1E);
    TEST("OpcodeFX29", test_OpcodeFX29);
    TEST("OpcodeFX33", test_OpcodeFX33);
    TEST("OpcodeFX55", test_OpcodeFX55);
    TEST("OpcodeFX65", test_OpcodeFX65);
    return 0;
}
