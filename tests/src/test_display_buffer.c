#include "test_display_buffer.h"
#include "display_buffer.h"
#include "test_macro.h"
#include "definitions.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>



void test_get_nth_bit(){
    uint8_t buffer[TEST_BUFFER_LEN];
    memset(buffer, 0, TEST_BUFFER_LEN);
    buffer[0] = 4;
    assert(get_nth_bit(buffer, 5) == 1);
}

void test_set_nth_bit(){
    uint8_t buffer[TEST_BUFFER_LEN];
    memset(buffer, 0, TEST_BUFFER_LEN);
    set_nth_bit(buffer, 2, 1);
    assert(get_nth_bit(buffer, 2) == 1);
    memset(buffer, 255, TEST_BUFFER_LEN);
    set_nth_bit(buffer, 2, 0);
    assert(get_nth_bit(buffer, 2) == 0);
}

void test_get_nth_byte(){
    uint8_t buffer[TEST_BUFFER_LEN];
    memset(buffer, 0, TEST_BUFFER_LEN);
    buffer[0] = 3;
    buffer[1] = 129;
    assert(get_nth_byte(buffer, 6) == 224);
    memset(buffer, 255, TEST_BUFFER_LEN);
    assert(get_nth_byte(buffer, 3) == 255);
}

void test_set_nth_byte(){
    {
        uint8_t buffer[TEST_BUFFER_LEN];
        memset(buffer, 0, TEST_BUFFER_LEN);
        set_nth_byte(buffer, 10, 255);
        assert(get_nth_byte(buffer, 10) == 255);
    }
    {
        uint8_t buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        set_nth_byte(buffer, 64, 129);
        assert(get_nth_byte(buffer, 64) == 129);
        set_nth_byte(buffer, 1921, 9);
        assert(get_nth_byte(buffer, 1921) == 9);
    }
    {
        uint8_t buffer[BUFFER_SIZE];
        memset(buffer, 255, BUFFER_SIZE);
        set_nth_byte(buffer, 1980, 0);
        assert(get_nth_byte(buffer, 1980) == 0);
    }
}

void test_get_byte_row_col(){
    uint8_t buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    set_nth_bit(buffer, 63, 1);
    set_nth_bit(buffer, 0, 1);
    assert(get_byte_row_col(buffer, 0, 63) == 192);
}
void test_set_byte_row_col(){
    uint8_t buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    set_byte_row_col(buffer, 0, 1, 20);
    assert(get_byte_row_col(buffer, 0, 1) == 20);
    set_byte_row_col(buffer, 0, 57, 129);
    assert(get_byte_row_col(buffer, 0, 57) == 129);
    set_byte_row_col(buffer, 10, 60, 129);
    assert(get_byte_row_col(buffer, 10, 60) == 129);
    set_byte_row_col(buffer, 20, 1, 129);
    assert(get_byte_row_col(buffer, 20, 1) == 129);
    memset(buffer, 255, BUFFER_SIZE);
    set_byte_row_col(buffer, 30, 61, 120);
    assert(get_byte_row_col(buffer, 30, 61) == 120);
}

int main(){
    TEST("get_nth_bit", test_get_nth_bit);
    TEST("set_nth_bit", test_set_nth_bit);
    TEST("get_nth_byte", test_get_nth_byte);
    TEST("set_nth_byte", test_set_nth_byte);
    TEST("get_byte_row_col", test_get_byte_row_col);
    TEST("set_byte_row_col", test_set_byte_row_col);
}


