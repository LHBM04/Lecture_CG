#ifndef GUARD_TEXTURE_H
#define GUARD_TEXTURE_H

#include "PCH.h"

class Texture
{
public:
    // 생성자: 파일 경로를 받아 텍스쳐 로드
    explicit Texture(const std::string& path);
    ~Texture();

    // 텍스쳐 바인딩 (슬롯 설정 가능)
    void Bind(unsigned int slot = 0) const;

    // 텍스쳐 언바인딩
    void Unbind() const;

    // 텍스쳐 생성 유틸리티
    [[nodiscard]]
    static std::unique_ptr<Texture> LoadFrom(const std::string& path);

private:
    unsigned int rendererID; // OpenGL 텍스쳐 ID
    std::string filePath;
    int width, height, bpp;
};

#endif // !GUARD_TEXTURE_H