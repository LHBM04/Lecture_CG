#ifndef GUARD_FILE_H
#define GUARD_FILE_H

#include <string>
#include <filesystem>

/**
 * @brief 지정한 경로 내 파일을 읽어옵니다.
 *
 * @param path_ 지정할 경로.
 *
 * @return std::string 파일 내용.
 */
std::string ReadFile(const std::filesystem::path& path_) noexcept;

#endif // !GUARD_FILE_H