#include "display.h"
#include "display_buffer.h"
#include <stdint.h>
static const color_t white = {0, 0, 0, 255};

static const color_t black = {255, 255, 255, 0};

void set_sdl_render_draw_color(SDL_Renderer* renderer, color_t* color){
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
}

display_t init_display(uint8_t* buffer, size_t buffer_len){
    display_t display;
    display.buffer = buffer;
    display.background = black;
    display.color = white;
    display.keypad = 0;
    memset(buffer, 0, buffer_len);
    SDL_Window *window;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        exit(1);
    }
    window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        printf("Window creation failed: %s\n", SDL_GetError());
        exit(1);
        SDL_Quit();
    }
    display.window = window;

    SDL_Renderer *renderer;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    display.renderer = renderer;
    return display;
}


uint8_t get_key(display_t* display, uint8_t key){
    return (display->keypad & (1 << key)) >> key;
}

void set_key(display_t* display, uint8_t key, uint8_t n){
    if(n){
        display->keypad |= (1 << key);
    } else {
        display->keypad &= ~(1 << key);
    }
}

void render(display_t* display){
    set_sdl_render_draw_color(display->renderer, &display->background);
    SDL_RenderClear(display->renderer);
    set_sdl_render_draw_color(display->renderer, &display->color);
    for(uint8_t y = 0; y < CHIP8_HEIGHT; y++){
        for(uint8_t x = 0; x < CHIP8_WIDTH; x++){
            if(get_nth_bit(display->buffer, y * CHIP8_WIDTH + x)){
                SDL_Rect pixelRect = { x * SCALE_FACTOR, y * SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR };
                SDL_RenderFillRect(display->renderer, &pixelRect);
            }
        }
    }
    SDL_RenderPresent(display->renderer);
}

void destroy_display(display_t* display){
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
}



static const SDL_Keycode chip8Keymap[CHIP8_KEY_COUNT] = {
    SDLK_1, SDLK_2, SDLK_3, SDLK_4,
    SDLK_q, SDLK_w, SDLK_e, SDLK_r,
    SDLK_a, SDLK_s, SDLK_d, SDLK_f,
    SDLK_z, SDLK_x, SDLK_c, SDLK_v
};

void event_handler(display_t* display){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        } else if(event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) {
            SDL_Keycode key = event.key.keysym.sym;
            for (int i = 0; i < CHIP8_KEY_COUNT; i++) {
                if (key == chip8Keymap[i]) {
                    set_key(display, i, event.type == SDL_KEYDOWN);
                    break;
                }
            }
        }
    }
}
