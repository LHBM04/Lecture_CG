#ifndef GUARD_APPLICATION_H
#define GUARD_APPLICATION_H

#include <filesystem>
#include <string>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/vec3.hpp>

/**
 * @brief 애플리케이션을 초기화합니다.
 */
void Initialize() noexcept;

/**
 * @brief 애플리케이션을 실행합니다.
 */
void Run() noexcept;

/**
 * @brief 애플리케이션을 업데이트합니다.
 */
void Update() noexcept;

/**
 * @brief 애플리케이션을 렌더링합니다.
 */
void Render() noexcept;

/**
 * @brief 애플리케이션을 재설정합니다.
 */
void Reset() noexcept;

/**
 * @brief 키와 상호작용할 때 호출됩니다.
 *
 * @param window_   윈도우.
 * @param key_      눌린 키.
 * @param scancode_ 스캔 코드.
 * @param action_   키 액션.
 * @param mods_     키 상태.
 */
void OnKeyInteracted(GLFWwindow* window_,
                     int         key_,
                     int         scancode_,
                     int         action_,
                     int         mods_) noexcept;

/**
 * @brief 버튼과 상호작용할 떄 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param button_ 클릭된 버튼.
 * @param action_ 버튼 액션.
 * @param mods_   버튼 상태.
 */
void OnButtonInteracted(GLFWwindow* window_,
                        int         button_,
                        int         action_,
                        int         mods_) noexcept;

/**
 * @brief 마우스 커서가 이동할 때 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param x_      마우스 커서의 X 좌표.
 * @param y_      마우스 커서의 Y 좌표.
 */
void OnCursorMoved(GLFWwindow* window_,
                   double      x_,
                   double      y_) noexcept;

/**
 * @brief 디버깅 메세지가 발생했을 때 호출됩니다.
 *
 * @param source_    메시지 출처.
 * @param type_      메시지 유형.
 * @param id_        메시지 ID.
 * @param severity_  메시지 심각도.
 * @param length_    메시지 길이.
 * @param message_   메시지 내용.
 * @param userParam_ 사용자 매개변수.
 */
void OnDebugMessage(GLenum       source_,
                    GLenum       type_,
                    GLuint       id_,
                    GLenum       severity_,
                    GLsizei      length_,
                    const char*  message_,
                    const void*  userParam_) noexcept;

/**
 *
 * @param path_
 */
std::string ReadFile(const std::filesystem::path& path_);

/**
 * @brief 애플리케이션 너비.
 */
constexpr unsigned int WINDOW_WIDTH  = 1200;

/**
 * @brief 애플리케이션 높이.
 */
constexpr unsigned int WINDOW_HEIGHT = 600;

/**
 * @brief OpenGL 컨텍스트 주 버전
 */
constexpr int CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief OpenGL 컨텍스트 부 버전
 */
constexpr int CONTEXT_MINOR_VERSION = 5;

/**
 * @brief 윈도우 타이틀
 */
constexpr const char* WINDOW_TITLE = "Level 01 - Act 13";

/**
 * @brief 최대 시간 배율.
 */
constexpr float MAX_TIME_SCALE = 30.0f;

/**
 * @brief 최소 시간 배율.
 */
constexpr float MIN_TIME_SCALE = 1.0f;

#endif // !GUARD_APPLICATION_H