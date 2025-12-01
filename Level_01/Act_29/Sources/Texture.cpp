#include "Texture.h"

// stb_image 구현부 포함 (PCH에 헤더만 있다면 여기서 define 필요, 이미 PCH에 구현부까지 있다면 생략 가능하나 보통 cpp에 둠)
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& path)
    : rendererID(0), filePath(path), width(0), height(0), bpp(0)
{
    // OpenGL 텍스쳐 좌표계(Y축)에 맞춰 이미지 뒤집기
    stbi_set_flip_vertically_on_load(1);

    unsigned char* localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);

    glGenTextures(1, &rendererID);
    glBindTexture(GL_TEXTURE_2D, rendererID);

    // 텍스쳐 파라미터 설정 (필터링, 래핑 등)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (localBuffer)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(localBuffer);
    }
    else
    {
        spdlog::error("Failed to load texture: {}", path);
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &rendererID);
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, rendererID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

std::unique_ptr<Texture> Texture::LoadFrom(const std::string& path)
{
    return std::make_unique<Texture>(path);
}