#ifndef __TEST_CHIP8_H__
#define __TEST_CHIP8_H__
#include "chip_8.h"
#include <stdint.h>
#define TEST_INSTRUCTION_LOCATION 0



chip_8_t get_test_chip(uint16_t instruction);

void test_Opcode00EE();

void test_Opcode00E0();

void test_Opcode1NNN();

void test_Opcode3XKK();

void test_Opcode4XKK();

void test_Opcode5XY0();

void test_Opcode6XKK();

void test_Opcode7XKK();

void test_Opcode8XY0();

void test_Opcode8XY1();

void test_Opcode8XY2();

void test_Opcode8XY3();

void test_Opcode8XY4_no_carry();

void test_Opcode8XY4_carry();

void test_Opcode8XY5_no_borrow();

void test_Opcode8XY5_borrow();

void test_Opcode8XY6_flag();

void test_Opcode8XY6_no_flag();

void test_Opcode8XY7_borrow();

void test_Opcode8XY7_no_borrow();

void test_Opcode8XYE_flag();

void test_Opcode8XYE_no_flag();

void test_Opcode9XY0_eq();

void test_Opcode9XY0_neq();

void test_OpcodeANNN();

void test_OpcodeBNNN();

void test_OpcodeDXYN();

#endif // ! __TEST_CHIP8_H__
