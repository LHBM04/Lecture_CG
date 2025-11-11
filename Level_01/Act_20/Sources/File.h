#ifndef GUARD_FILE_H
#define GUARD_FILE_H

#include <cstddef>
#include <filesystem>
#include <string>

class File final
{
public:
    /**
     * @brief 지정한 경로 내 파일을 읽어옵니다.
     *
     * @param path_ 지정할 경로.
     *
     * @return std::string 파일 내용.
     */
    static std::string ReadFile(const std::filesystem::path& path_) noexcept;

private:
    File() = delete;
    ~File() = delete;

    File(const File&) = delete;
    File(File&&) = delete;

    File& operator=(const File&) = delete;
    File& operator=(File&&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;
    void  operator delete(void*) = delete;
    void  operator delete[](void*) = delete;
};

#endif // !GUARD_FILE_H