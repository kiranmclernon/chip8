#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <stdint.h>
#include <math.h>
#include <SDL2/SDL.h>

#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32
#define SCALE_FACTOR 10
#define SCREEN_WIDTH CHIP8_WIDTH * SCALE_FACTOR
#define SCREEN_HEIGHT CHIP8_HEIGHT * SCALE_FACTOR
#define BUFFER_LEN (CHIP8_WIDTH * CHIP8_HEIGHT / 8 + 1)
#define CHIP8_KEY_COUNT 16

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} color_t;

typedef struct{
    SDL_Window* window;
    SDL_Renderer* renderer;
    uint8_t* buffer;
    color_t color;
    color_t background;
    uint8_t* open;
    uint16_t keypad;
} display_t;

void set_sdl_render_draw_color(SDL_Renderer* renderer, color_t* color);

display_t init_display(uint8_t* buffer, size_t buffer_len, uint8_t* open);

uint8_t get_key(display_t* display, uint8_t key);

void set_key(display_t* display, uint8_t key, uint8_t n);

void render(display_t* display);

void destroy_display(display_t* display);

void event_handler(display_t* display);



#endif // ! _DISPLAY_H_
