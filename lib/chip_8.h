#ifndef __CHIP8_H__
#define __CHIP8_H__
#include <stdint.h>
#include "definitions.h"
#include "display.h"


typedef struct{
    uint8_t memory[MEM_SIZE];
    uint16_t stack[STACK_SIZE];
    uint8_t registers[N_REG];
    uint8_t sp;
    uint16_t pc;
    uint8_t* Vf;
    uint16_t I;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t display_buffer[BUFFER_SIZE];
    display_t display;
} chip_8_t;


typedef struct{
    uint16_t instruction;
    uint8_t n0;
    uint8_t n1;
    uint8_t n2;
    uint8_t n3;
} instruction_t;



chip_8_t create_chip8();

int load_chip8(chip_8_t* chip, const char* path);

void destroy_chip8(chip_8_t* chip);

typedef void(*op)(chip_8_t*, instruction_t*);

instruction_t fetch(chip_8_t*);

op decode_instruction(instruction_t inst);

void Opcode00EE(chip_8_t*, instruction_t*); // DONE

void Opcode00E0(chip_8_t*, instruction_t*); // DONE

void Opcode1NNN(chip_8_t*, instruction_t*); // DONE

void Opcode2NNN(chip_8_t*, instruction_t*); // DONE

void Opcode3XKK(chip_8_t*, instruction_t*); // DONE

void Opcode4XKK(chip_8_t*, instruction_t*); // DONE

void Opcode5XY0(chip_8_t*, instruction_t*); // DONE

void Opcode6XKK(chip_8_t*, instruction_t*); // DONE

void Opcode7XKK(chip_8_t*, instruction_t*); // DONE

void Opcode8XY0(chip_8_t*, instruction_t*); // DONE

void Opcode8XY1(chip_8_t*, instruction_t*); // DONE

void Opcode8XY2(chip_8_t*, instruction_t*); // DONE

void Opcode8XY3(chip_8_t*, instruction_t*); // DONE

void Opcode8XY4(chip_8_t*, instruction_t*); // DONE

void Opcode8XY5(chip_8_t*, instruction_t*); // DONE

void Opcode8XY6(chip_8_t*, instruction_t*); // DONE

void Opcode8XY7(chip_8_t*, instruction_t*); // DONE

void Opcode8XYE(chip_8_t*, instruction_t*); // DONE

void Opcode9XY0(chip_8_t*, instruction_t*); // DONE

void OpcodeANNN(chip_8_t*, instruction_t*); // DONE

void OpcodeBNNN(chip_8_t*, instruction_t*); // DONE 

void OpcodeCXNN(chip_8_t*, instruction_t*); //DONE

void OpcodeDXYN(chip_8_t*, instruction_t*); // DONE

void OpcodeEX9E(chip_8_t*, instruction_t*); // DONE

void OpcodeEXA1(chip_8_t*, instruction_t*); // DONE

void OpcodeFX07(chip_8_t*, instruction_t*); // DONE

void OpcodeFX0A(chip_8_t*, instruction_t*); // DONE

void OpcodeFX15(chip_8_t*, instruction_t*); // DONE

void OpcodeFX18(chip_8_t*, instruction_t*); // DONE

void OpcodeFX1E(chip_8_t*, instruction_t*); // DONE

void OpcodeFX29(chip_8_t*, instruction_t*); // DONE
 
void OpcodeFX33(chip_8_t*, instruction_t*); // DONE

void OpcodeFX55(chip_8_t*, instruction_t*); // DONE

void OpcodeFX65(chip_8_t*, instruction_t*); // DONE

void Opcode0000(chip_8_t*, instruction_t*);

uint8_t random_number();

#endif // !__CHIP8_H__
