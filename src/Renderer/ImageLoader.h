#pragma once
#include <cstdint>
#include <filesystem>
#include "GL/glew.h"

class ImageTexture
{
  private:
    GLuint m_Texture = 0;
    int m_Width = 0;
    int m_Height = 0;
    bool m_Initialized = false;

  public:
    ImageTexture() {}
    ImageTexture(const char *filename);
    ImageTexture(const std::filesystem::path &filename);
    ImageTexture(const uint8_t *image_data, int image_width, int image_height);
    ~ImageTexture();

    bool Update(const char *filename);
    bool Update(const uint8_t *new_image_data, int new_image_width, int new_image_height);

    inline GLuint Texture() const { return m_Texture; }
    inline int Width() const { return m_Width; }
    inline int Height() const { return m_Height; }
    inline bool IsInitialized() const { return m_Initialized; }
};

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height);

bool LoadTextureFromMemory(const uint8_t *image_data, const int image_width, const int image_height,
                           GLuint *out_texture, int *out_width, int *out_height);

bool UpdateTextureFromMemory(const uint8_t *new_image_data, const int new_image_width,
                             const int new_image_height, const GLuint image_texture,
                             const int image_width, const int image_height);