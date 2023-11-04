#include "test_display.h"
#include "display.h"
#include "assert.h"
#include "test_macro.h"
display_t test_display(){
    uint8_t* buffer = malloc(sizeof(uint8_t) * BUFFER_SIZE);
    display_t display;
    display.buffer = buffer;
    display.keypad = 0;
    return display;
}

void free_test_display(display_t* display){
    free(display->buffer);
}


void test_set_key(){
    display_t display = test_display();
    set_key(&display, 15, 1);
    assert(display.keypad == 0x8000);
    set_key(&display, 15, 0);
    assert(display.keypad == 0);
    set_key(&display, 0, 1);
    assert(display.keypad == 1);
    set_key(&display, 0, 0);
    assert(display.keypad == 0);
    free_test_display(&display);
}

void test_get_key(){
    display_t display = test_display();
    for(int i = 0; i < 15; i++){
        set_key(&display, i, 1);
        assert(get_key(&display, i) != 0);
    }
    free_test_display(&display);

}

int main(){
    TEST("set_key", test_set_key);
    TEST("get_key", test_get_key);
}
