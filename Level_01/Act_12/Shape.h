#ifndef GUARD_SHAPE_H
#define GUARD_SHAPE_H

#include <array>
#include <map>

struct Shape
{
public:
    enum class Type
    {
        Line,
        Triangle,
        Quadrant,
        Pentagon
    };

    /**
     * @brief 생성자.
     */
    explicit Shape() noexcept;

    /**
     * @brief 소멸자.
     */
    ~Shape() noexcept;
private:
    /**
     * @brief 정점 데이터.
     */
    unsigned int vao;

    /**
     * @brief 정점 버퍼 객체.
     */
    unsigned int vbo;

    /**
     * @brief 요소 배열 객체.
     */
    unsigned int ebo;

    /**
     * @brief 현재 도형 타입.
     */
    Shape::Type currentType;
};

/**
 * @brief 각 도형의 정점 데이터.
 */
static const std::map<Shape::Type, std::array<int, 10>> ShapeVertices
{

};

/**
 * @brief 각 도형의 인덱스 데이터.
 */
static const std::map<Shape::Type, std::array<int, 10>> ShapeIndices
{

};

#endif // !GUARD_SHAPE_H