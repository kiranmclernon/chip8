#include "chip_8.h"
#include "display.h"
#include "display_buffer.h"
#include "definitions.h"


int main(){
    chip_8_t chip8 = create_chip8();
    load_chip8(&chip8, "examples/2-ibm-logo.ch8");
    uint8_t display_buffer[BUFFER_SIZE];
    memset(display_buffer, 255, 8);
    while(1) {
        event_handler(&chip8.display);
        instruction_t inst = fetch(&chip8);
        decode_instruction(inst)(&chip8, &inst);
        render(&chip8.display);
    }
}
