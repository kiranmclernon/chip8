#include "display_buffer.h"
#include <stdint.h>


uint8_t get_nth_bit(uint8_t* array, uint16_t n){
    uint16_t byte = n / 8;
    uint8_t index = n % 8;
    return (array[byte] & (1 << index)) != 0;
}

void set_nth_bit(uint8_t* buffer, uint16_t n, uint8_t val){
    uint16_t byte = n / 8;
    uint8_t index = n % 8;
    if(val){
        buffer[byte] |= (1 << index);
    } else{
        buffer[byte] &= ~(1 << index);
    }
}

uint8_t get_nth_byte(uint8_t* array, uint16_t n){
    uint16_t byte = n / 8;
    uint8_t offset = n % 8;
    uint8_t mask = ~((1 << offset) - 1);
    return (array[byte] >> offset) | ((array[byte+1] & mask) >> (8 - offset));
}


void set_nth_byte(uint8_t* array, uint16_t n, uint8_t c){
    uint16_t byte = n / 8;
    uint8_t offset = n % 8;
    uint8_t mask = ~(0xFF >> offset);
    array[byte] &= mask;
    array[byte + 1] &= ~mask;
    array[byte] |= c >> offset;
    array[byte] |= c << (8 - offset);
}


uint8_t set_nth_byte_with_flip(uint8_t* array, uint16_t n, uint8_t c){
    uint8_t original = get_nth_byte(array, n);
    uint16_t byte = n / 8;
    uint8_t offset = n % 8;
    uint8_t mask = ~(0xFF >> offset);
    array[byte] &= mask;
    array[byte + 1] &= ~mask;
    array[byte] |= c >> offset;
    array[byte] |= c << (8 - offset);
    return original & ~get_nth_byte(array, n);
}
