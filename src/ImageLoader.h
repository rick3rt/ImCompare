#pragma once
#include <cstdint>
#include "GL/glew.h"

struct ImageTexture
{
    GLuint texture = 0;
    int width = 0;
    int height = 0;
    bool initialized = false;

    ImageTexture(const char *filename);
    ImageTexture(const uint8_t *image_data, int image_width, int image_height);
    ~ImageTexture();
    bool Update(const char *filename);
    bool Update(const uint8_t *new_image_data, int new_image_width, int new_image_height);
};

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height);

bool LoadTextureFromMemory(const uint8_t *image_data, const int image_width, const int image_height,
                           GLuint *out_texture, int *out_width, int *out_height);

bool UpdateTextureFromMemory(const uint8_t *new_image_data, const int new_image_width,
                             const int new_image_height, const GLuint image_texture,
                             const int image_width, const int image_height);