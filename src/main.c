#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"

vec3_t camera_position = { 0, 0, -5 }; 
vec3_t cube_rotation = { 0, 0, 0 };

triangle_t* triangles_to_render = NULL;

float fov_factor = 1280;

bool is_running = false;
uint64_t previous_frame_time = 0;

void setup(void) {
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
    if (color_buffer == NULL) {
        fprintf(stderr, "Error initializing memory for color buffer.\n");
        exit(-1);
    }

    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {  
                is_running = false;
            }
            break;
    }
}

vec2_t project(vec3_t vertex) {
    vec2_t projected_vertex = { 
        (fov_factor * vertex.x) / vertex.z, 
        (fov_factor * vertex.y) / vertex.z
    };

    return projected_vertex;
}

void update(void) {
    int current_time = SDL_GetTicks64();
    int elapsed_time = current_time - previous_frame_time;
    int time_to_wait = FRAME_TARGET_TIME - elapsed_time;

    if (time_to_wait > 0) {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = current_time + time_to_wait;

    triangles_to_render = NULL;

    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    for (int i = 0; i < N_MESH_FACES; i++) {
        face_t mesh_face = mesh_faces[i];
        
        vec3_t face_vertices[3];
        face_vertices[0] = mesh_vertices[mesh_face.a - 1];
        face_vertices[1] = mesh_vertices[mesh_face.b - 1];
        face_vertices[2] = mesh_vertices[mesh_face.c - 1];

        triangle_t projected_triangle;

        for (int j = 0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];

            transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

            transformed_vertex.z -= camera_position.z;
            vec2_t projected_vertex = project(transformed_vertex);

            // Move the vertexes toward the center of the window
            projected_vertex.x += (WINDOW_WIDTH / 2);
            projected_vertex.y += (WINDOW_HEIGHT / 2);

            projected_triangle.points[j] = projected_vertex;
        }

        array_push(triangles_to_render, projected_triangle);
    }
}

void render(void) {
    render_color_buffer();
    clear_color_buffer(0xFF000000);

    int num_triangles = array_length(triangles_to_render);

    for (int i = 0; i < num_triangles; i++) {
        triangle_t triangle = triangles_to_render[i];

        draw_triangle(triangle);
    } 

    array_free(triangles_to_render);

    SDL_RenderPresent(renderer);
}

int main() {
    is_running = initalize_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}