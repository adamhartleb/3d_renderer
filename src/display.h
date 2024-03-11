#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "triangle.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

bool initalize_window(void);
void clear_color_buffer(uint32_t color);
void render_color_buffer(void);
void destroy_window(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_line(int x1, int y1, int x2, int y2, uint32_t color);
void draw_triangle(triangle_t triangle);

#endif