#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "display.h"
#include "triangle.h"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

bool initalize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    WINDOW_WIDTH = display_mode.w;
    WINDOW_HEIGHT = display_mode.h;

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );

    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    int fullscreen = SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    if (fullscreen != 0) {
        fprintf(stderr, "Error setting window to fullscreen.\n");
        return false;
    }

    return true;
}

void clear_color_buffer(uint32_t color) {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            color_buffer[(WINDOW_WIDTH * y) + x] = color;
        }
    }
}

void render_color_buffer(void) {
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(WINDOW_WIDTH * sizeof(uint32_t))
    );

    SDL_RenderCopy(
        renderer,
        color_buffer_texture,
        NULL,
        NULL
    );
}

void draw_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
        color_buffer[(WINDOW_WIDTH * y) + x] = color;
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int current_x = x + i;
            int current_y = y + j;
            draw_pixel(current_x, current_y, color);
        }
    } 
}

void draw_line(int x1, int y1, int x2, int y2, uint32_t color) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float side_length, current_x, current_y, x_increment, y_increment;

    side_length = abs(dx) > abs(dy) ? abs(dx) : abs(dy); 

    x_increment = dx / side_length;
    y_increment = dy / side_length;

    current_x = x1;
    current_y = y1;

    for (int i = 0; i <= side_length; i++) {
        draw_pixel(round(current_x), round(current_y), color);
        current_x += x_increment;
        current_y += y_increment;
    }
}

void draw_triangle(triangle_t triangle) {
    draw_line(
        triangle.points[0].x,
        triangle.points[0].y,
        triangle.points[1].x,
        triangle.points[1].y,
        0xFFFFFF00
    );
    draw_line(
        triangle.points[1].x,
        triangle.points[1].y,
        triangle.points[2].x,
        triangle.points[2].y,
        0xFFFFFF00
    );
    draw_line(
        triangle.points[2].x,
        triangle.points[2].y,
        triangle.points[0].x,
        triangle.points[0].y,
        0xFFFFFF00
    );
}

void destroy_window(void) {
    free(color_buffer);
    color_buffer = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
