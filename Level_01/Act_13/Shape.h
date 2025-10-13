#ifndef GUARD_SHAPE_H
#define GUARD_SHAPE_H

#include <array>
#include <unordered_map>

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Shader.h"

/**
 * @class Shape
 *
 * @brief 도형을 정의합니다.
 */
class Shape final
{
public:
    enum class Type : unsigned char
    {
        /**
         * @brief 점.
         */
        Dot,

        /**
         * @brief 선.
         */
        Line,

        /**
         * @brief 삼각형.
         */
        Triangle,

        /**
         * @brief 사각형.
         */
        Rectangle,

        /**
         * @brief 오각형.
         */
        Pentagon
    };

    /**
     * @brief 생성자.
     *
     * @param type      도형의 타입.
     * @param position_ 도형의 위치.
     */
    explicit Shape(Shape::Type type,
                   glm::vec2   position_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Shape() noexcept;

    /**
     * @brief 도형의 위치를 반환합니다.
     *
     * @return position_ 도형의 위치.
     */
    [[nodiscard]]
    constexpr glm::vec2 GetPosition() const noexcept
    {
        return position;
    }

    /**
     * @brief 도형의 위치를 설정합니다.
     *
     * @param position_ 설정할 위치.
     */
    inline void SetPosition(const glm::vec2 position_) noexcept
    {
        position = position_;
    }

    /**
     * @brief 도형의 크기를 반환합니다.
     *
     * @return scale_ 도형의 크기.
     */
    [[nodiscard]]
    constexpr float GetScale() const noexcept
    {
        return scale;
    }

    /**
     * @brief 도형의 크기를 설정합니다.
     *
     * @param scale_ 설정할 크기.
     */
    inline void SetScale(const float scale_) noexcept
    {
        scale = scale_;
    }

    /**
     * @brief 도형의 방향을 반환합니다.
     *
     * @return direction_ 도형의 방향.
     */
    [[nodiscard]]
    constexpr glm::vec2 GetDirection() const noexcept
    {
        return direction;
    }

    /**
     * @brief 도형의 위치를 설정합니다.
     *
     * @param direction_ 설정할 위치.
     */
    inline void SetDirection(const glm::vec2 direction_) noexcept
    {
        direction = direction_;
    }

    /**
     * @brief 도형이 파괴되어야 하는지에 대한 여부를 반환합니다.
     *
     * @return bool 파괴 여부.
     */
    [[nodiscard]]
    inline bool ShouldDestroy() const noexcept
    {
        return shouldDestroy;
    }

    /**
     * @brief 도형을 파괴 상태로 설정합니다.
     */
    inline void Destroy() noexcept
    {
        shouldDestroy = true;
    }

    /**
     * @brief 도형을 업데이트합니다.
     *
     * @param deltaTime_ 시간 변화량.
     */
    void Update(float deltaTime_) noexcept;

    /**
     * @brief 도형을 그립니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    void Render(const Shader& shader_) const noexcept;

    /**
     * @brief 도형을 다음 위치로 이동시킵니다.
     *
     * @param other_ 이동할 도형.
     */
    void NextTo(const Shape& other_) noexcept;

    /**
     * @brief 지정한 점과 접촉했는지에 대한 여부를 반환합니다.
     *
     * @return point_ 지정할 점.
     */
    [[nodiscard]]
    constexpr bool IsInteracted(const glm::vec2 point_) const noexcept
    {
        const glm::vec2 min = { position.x - scale * 0.5f, position.y - scale * 0.5f };
        const glm::vec2 max = { position.x + scale * 0.5f, position.y + scale * 0.5f };

        return (point_.x >= min.x) && (point_.x <= max.x) &&
               (point_.y >= min.y) && (point_.y <= max.y);
    }

    /**
     * @brief 지정한 도형과 접촉했는지에 대한 여부를 반환합니다.
     *
     * @return other_ 지정할 도형.
     */
    [[nodiscard]]
    constexpr bool IsInteracted(const Shape& other_) const noexcept
    {
        const glm::vec2 min = { position.x - scale * 0.5f, position.y - scale * 0.5f };
        const glm::vec2 max = { position.x + scale * 0.5f, position.y + scale * 0.5f };

        const glm::vec2 otherMin = { other_.position.x - other_.scale * 0.5f, other_.position.y - other_.scale * 0.5f };
        const glm::vec2 otherMax = { other_.position.x + other_.scale * 0.5f, other_.position.y + other_.scale * 0.5f };

        return (min.x < otherMax.x) && (max.x > otherMin.x) &&
               (min.y < otherMax.y) && (max.y > otherMin.y);
    }
private:
    /**
     * @brief 도형의 모든 유형에 대한 정점 데이터.
     */
    static inline const std::unordered_map<Shape::Type, std::array<float, 10>> SHAPES_VERTICES
    {
            {Shape::Type::Dot,       {0.0f, 0.0f}},
            {Shape::Type::Line,      {-0.5f, 0.0f,  0.5f,    0.0f}},
            {Shape::Type::Triangle,  { 0.0f, 0.5f, -0.5f,   -0.5f,    0.5f,    -0.5f}},
            {Shape::Type::Rectangle, {-0.5f, 0.5f,  0.5f,    0.5f,    0.5f,    -0.5f, -0.5f, -0.5f}},
            {Shape::Type::Pentagon,  { 0.0f, 0.5f,  0.4755f, 0.1545f, 0.2939f, -0.4045f, -0.2939f, -0.4045f, -0.4755f, 0.1545f}},
    };

    /**
     * @brief 도형의 모든 유형에 대한 요소 데이터.
     */
    static inline const std::unordered_map<Shape::Type, std::vector<unsigned int>> SHAPE_INDICES
    {
            {Shape::Type::Dot,       {0}},
            {Shape::Type::Line,      {0, 1}},
            {Shape::Type::Triangle,  {0, 1, 2}},
            {Shape::Type::Rectangle, {0, 1, 2, 2, 3, 0}},
            {Shape::Type::Pentagon,  {0, 1, 2, 0, 2, 3, 0, 3, 4}},
    };

    /**
     * @brief 도형의 모든 유형에 대한 그리기 기본형 데이터.
     */
    static inline const std::unordered_map<Shape::Type, GLenum> SHAPE_PRIMITIVES
    {
         {Shape::Type::Dot,       GL_POINTS},
         {Shape::Type::Line,      GL_LINES},
         {Shape::Type::Triangle,  GL_TRIANGLES},
         {Shape::Type::Rectangle, GL_TRIANGLES},
         {Shape::Type::Pentagon,  GL_TRIANGLES},
    };

    /**
     * @brief 도형의 모든 유형에 대한 색상 데이터.
     */
    static inline const std::unordered_map<Shape::Type, glm::vec3> SHAPE_COLORS
    {
            {Shape::Type::Dot,       {1.0f, 1.0f, 1.0f}},
            {Shape::Type::Line,      {1.0f, 0.0f, 0.0f}},
            {Shape::Type::Triangle,  {0.0f, 1.0f, 0.0f}},
            {Shape::Type::Rectangle, {0.0f, 0.0f, 1.0f}},
            {Shape::Type::Pentagon,  {1.0f, 1.0f, 0.0f}},
    };

    /**
     * @brief 정점 배열 객체.
     */
    GLuint vao = 0;

    /**
     * @brief 정점 버퍼 객체.
     */
    GLuint vbo = 0;

    /**
     * @brief 요소 버퍼 객체.
     */
    GLuint ebo = 0;

    /**
     * @brief 해당 도형의 타입.
     */
    Shape::Type type;

    /**
     * @brief 해당 도형의 위치.
     */
    glm::vec2 position;

    /**
     * @brief 해당 도형의 크기.
     */
    float scale;

    /**
     * @brief 해당 도형이 바라보는 각도.
     */
    glm::vec2 direction;

    /**
     * @brief 도형이 파괴되어야 하는지 여부.
     */
    bool shouldDestroy = false;
};

#endif // !GUARD_SHAPE_H
