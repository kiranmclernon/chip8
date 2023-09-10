#ifndef __DISPLAY_BUFFER_H__
#define __DISPLAY_BUFFER_H__
#include <stdint.h>

uint8_t get_nth_bit(uint8_t* array, uint16_t n);

void set_nth_bit(uint8_t* buffer, uint16_t n, uint8_t val);

uint8_t get_nth_byte(uint8_t* array, uint16_t n);

void set_nth_byte(uint8_t* array, uint16_t n, uint8_t c);

uint8_t set_nth_byte_with_flip(uint8_t* array, uint16_t n, uint8_t c);
#endif // !__DISPLAY_BUFFER_H__


