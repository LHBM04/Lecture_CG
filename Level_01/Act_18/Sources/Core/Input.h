#ifndef GUARD_INPUT_H
#define GUARD_INPUT_H

#include <array>
#include <cstddef>
#include <unordered_map>

#include <glm/vec2.hpp>

class Application;

/**
 * @class Input
 *
 * @brief 입력을 처리합니다.
 */
class Input final
{
    friend class Application;
public:
    /**
     * @brief 입력 상태를 업데이트합니다.
     */
    static void Update();

    /**
     * @brief 지정한 키가 눌렸는지 확인합니다.
     *
     * @param key_ 확인할 키 코드
     * @return bool 키가 눌렸다면 true, 그렇지 않다면 false
     */
    static bool IsKeyPressed(int key_) noexcept;

    /**
     * @brief 지정한 키가 눌린 상태인지 확인합니다.
     *
     * @param key_ 확인할 키 코드
     *
     * @return bool 키가 눌린 상태라면 true, 그렇지 않다면 false
     */
    static bool IsKeyHeld(int key_) noexcept;

    /**
     * @brief 지정한 키가 떼어졌는지 확인합니다.
     *
     * @param key_ 확인할 키 코드
     *
     * @return bool 키가 떼어졌다면 true, 그렇지 않다면 false
     */
    static bool IsKeyReleased(int key_) noexcept;

    /**
     * @brief 지정한 마우스 버튼이 눌렸는지 확인합니다.
     *
     * @param button_ 확인할 마우스 버튼 코드
     *
     * @return bool 마우스 버튼이 눌렸다면 true, 그렇지 않다면 false
     */
    static bool IsMouseButtonPressed(int button_) noexcept;

    /**
     * @brief 지정한 마우스 버튼이 눌린 상태인지 확인합니다.
     *
     * @param button_ 확인할 마우스 버튼 코드
     *
     * @return bool 마우스 버튼이 눌린 상태라면 true, 그렇지 않다면 false
     */
    static bool IsMouseButtonHeld(int button_) noexcept;

    /**
     * @brief 지정한 마우스 버튼이 떼어졌는지 확인합니다.
     *
     * @param button_ 확인할 마우스 버튼 코드
     *
     * @return bool 마우스 버튼이 떼어졌다면 true, 그렇지 않다면 false
     */
    static bool IsMouseButtonReleased(int button_) noexcept;

    /**
     * @brief 마우스 커서의 현재 위치를 반환합니다.
     *
     * @return glm::vec2 마우스 커서의 현재 위치
     */
    static glm::vec2 GetMousePosition() noexcept;

    /**
     * @brief 마우스 커서의 이동량을 반환합니다.
     *
     * @return glm::vec2 마우스 커서의 이동량
     */
    static glm::vec2 GetMouseDelta() noexcept;

private:
    Input() = delete;
    ~Input() = delete;

    Input(const Input&) = delete;
    Input(Input&&) = delete;

    Input& operator=(const Input&) = delete;
    Input& operator=(Input&&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;
    void  operator delete(void*) = delete;
    void  operator delete[](void*) = delete;

    /**
     * @brief 키가 눌렸을 때 호출됩니다.
     *
     * @param key_ 현재 눌린 키
     * @param x_   현재 마우스 커서의 X 좌표
     * @param y_   현재 마우스 커서의 Y 좌표
     */
    static void OnKeyPressed(unsigned char key_, int x_, int y_) noexcept;

    /**
     * @brief 키가 떼어졌을 때 호출됩니다.
     *
     * @param key_ 현재 떼어진 키
     * @param x_   현재 마우스 커서의 X 좌표
     * @param y_   현재 마우스 커서의 Y 좌표
     */
    static void OnKeyReleased(unsigned char key_, int x_, int y_) noexcept;

    /**
     * @brief 특수 키가 눌렸을 때 호출됩니다.
     *
     * @param key_ 현재 눌린 특수 키
     * @param x_   현재 마우스 커서의 X 좌표
     * @param y_   현재 마우스 커서의 Y 좌표
     */
    static void OnSpecialKeyPressed(int key_, int x_, int y_) noexcept;

    /**
     * @brief 특수 키가 떼어졌을 때 호출됩니다.
     *
     * @param key_ 현재 떼어진 특수 키
     * @param x_   현재 마우스 커서의 X 좌표
     * @param y_   현재 마우스 커서의 Y 좌표
     */
    static void OnSpecialKeyReleased(int key_, int x_, int y_) noexcept;

    /**
     * @brief 마우스 버튼이 클릭되었을 때 호출됩니다.
     *
     * @param button_ 클릭된 마우스 버튼
     * @param state_  마우스 버튼 상태
     * @param x_      현재 마우스 커서의 X 좌표
     * @param y_      현재 마우스 커서의 Y 좌표
     */
    static void OnMouseClicked(int button_, int state_, int x_, int y_) noexcept;

    /**
     * @brief 마우스가 이동했을 때 호출됩니다.
     *
     * @param x_ 현재 마우스 커서의 X 좌표
     * @param y_ 현재 마우스 커서의 Y 좌표
     */
    static void OnMouseMoved(int x_, int y_) noexcept;

    /**
     * @brief 현재 프레임의 키 상태 맵.
     */
    static std::unordered_map<int, bool> currentKeyStates;

    /**
     * @brief 이전 프레임의 키 상태 맵.
     */
    static std::unordered_map<int, bool> previousKeyStates;

    /**
     * @brief 현재 프레임의 마우스 버튼 상태 배열.
     */
    static std::array<bool, 8> currentMouseButtonStates;

    /**
     * @brief 이전 프레임의 마우스 버튼 상태 배열.
     */
    static std::array<bool, 8> previousMouseButtonStates;

    /**
     * @brief 현재 프레임의 마우스 커서 위치.
     */
    static glm::vec2 currentMousePosition;

    /**
     * @brief 이전 프레임의 마우스 커서 위치.
     */
    static glm::vec2 previousMousePosition;
};

#endif // !GUARD_INPUT_H