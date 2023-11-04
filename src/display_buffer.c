#include "display_buffer.h"
#include "display.h"
#include <stdint.h>
#include <stdio.h>


void print_bits_array(uint8_t* array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (array[i] >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

void print_display_buffer(uint8_t* buffer){
    for(int line = 0; line < CHIP8_HEIGHT; line++){
        print_bits_array(buffer + line * 8, 8);
    }
}
uint8_t get_nth_bit(uint8_t* array, uint16_t n){
    uint16_t byte = n / 8; // Ineger multiplication to get the byte
    uint8_t index = n % 8; // Get the bit
    return (array[byte] & (1 << (7 - index))) != 0; // Return the bit
}

void set_nth_bit(uint8_t* buffer, uint16_t n, uint8_t val){
    uint16_t byte = n / 8;
    uint8_t index = n % 8;
    if(val){
        buffer[byte] |= (1 << (7 - index));
    } else{
        buffer[byte] &= ~(1 << (7 - index));
    }
}

uint8_t get_nth_byte(uint8_t* array, uint16_t n){
    uint16_t byte = n / 8; // get the byte
    uint8_t offset = n % 8; //get the offset
    uint8_t mask = 0xFF >> (offset); // first n bits set
    uint8_t p1 = (array[byte] & mask) << offset;
    uint8_t p2 = (array[byte + 1] & ~(mask)) >> (8 - offset);
    return p1 | p2;
}

void set_nth_byte(uint8_t* array, uint16_t n, uint8_t c){
    uint16_t byte = n / 8;
    uint8_t offset = n % 8;
    uint8_t c1, c2;
    c1 = (c & (0xFF << offset)) >> (offset);
    c2 = c << (8 - offset);
    array[byte] &= (0xFF << (8-offset));
    array[byte + 1] &= (0xFF >> offset);
    array[byte] |= c1;
    array[byte + 1] |= c2;
}

uint16_t wrap_row(uint16_t value, uint8_t row) {
    return  row * CHIP8_WIDTH + ((value - row * CHIP8_WIDTH) % (CHIP8_WIDTH));
}

uint8_t get_end_byte(uint8_t* array, uint16_t end){
    uint16_t byte_start = end - (end % 64);
    return get_nth_byte(array,byte_start);
}
/*
 If there is a row in the buffer like this and we want to get the byte
 from the sixty third column
 10000000 00000000 00000000 00000000 00000000 00000000 00000000 00000001 
 This functions graps tha that byte and wraps aroudn giving the last bit
 and then also the first seven bits.
*/
uint8_t get_byte_row_col(uint8_t* array, uint8_t row, uint8_t col){
    uint16_t start = row * CHIP8_WIDTH + col;
    uint16_t end = wrap_row(start + 8, row);
    if(end - start == 8){
        return get_nth_byte(array, start);
    } else {
        uint8_t offset = 8 - (CHIP8_WIDTH - wrap_row(start, row)); // How many bits from start byte
        uint8_t b1 = get_nth_byte(array, start);
        uint8_t b2 = get_end_byte(array, end);
        b1 &= (0xFF << offset); // get the most 8 - offset significant bits from b1
        b2 &= (0xFF << (8 - offset)); // get the  offset siginifant bits from b2
        b2 >>= (8 - offset); // shift the bits in b2
        return b1 | b2; // combine
    }
}


void set_end_byte(uint8_t* array, uint16_t end, uint8_t c){
    uint16_t byte_start = end - (end % 64);
    set_nth_byte(array,byte_start, c);
}
void set_byte_row_col(uint8_t* array, uint8_t row, uint8_t col, uint8_t c){
    uint16_t start = row * CHIP8_WIDTH + col;
    uint16_t end = wrap_row(start + 8, row);
    if(end - start == 8){
        set_nth_byte(array, start, c);
    } else {
        uint8_t offset = 8 - (CHIP8_WIDTH - wrap_row(start, row)); // How many bits from start byte
        uint8_t b1 = get_nth_byte(array, start);
        uint8_t b2 = get_end_byte(array, end);
        uint8_t c1, c2;
        c1 = c & (0xFF << (8- offset));
        c2 = (c << (8 - offset));
        b1 &= (0xFF >> (8 - offset));
        b2 &= (0xFF >> offset);
        b1 |= c1;
        b2 |= c2;
        set_nth_byte(array, start, b1);
        set_end_byte(array, end, b2);
    }
}
