#ifndef GUARD_MODEL_H
#define GUARD_MODEL_H

#include <filesystem>

#include <glm/vec3.hpp>

/**
 * @class Model
 *
 * @brief 모델을 정의합니다.
 */
class Model final
{
public:
    /**
     * @brief 생성자.
     *
     * @param source_ 모델 소스
     */
    explicit Model(const char* source_) noexcept;

    /**
     * @brief 정점 데이터를 반환합니다.
     *
     * @return const std::vector<glm::vec3>& 정점 데이터.
     */
    [[nodiscard]]
    inline const std::vector<glm::vec3>& GetVertices() const noexcept
    {
        return vertices;
    }

    /**
     * @brief 법선 데이터를 반환합니다.
     *
     * @return const std::vector<glm::vec3>& 법선 데이터.
     */
    [[nodiscard]]
    inline const std::vector<glm::vec3>& GetNormals() const noexcept
    {
        return normals;
    }

    /**
     * @brief 면 데이터를 반환합니다.
     *
     * @return const std::vector<glm::vec3>& 면 데이터.
     */
    [[nodiscard]]
    inline const std::vector<glm::uvec3>& GetFaces() const noexcept
    {
        return faces;
    }
private:
    /**
     * @brief 정점 데이터 접두사
     */
    static constexpr const char* vertexPrefix = "v ";

    /**
     * @brief 법선 데이터 접두사.
     */
    static constexpr const char* normalPrefix = "vn";

    /**
     * @brief 면 데이터 접두사.
     */
    static constexpr const char* facePrefix = "f ";

    /**
     * @brief 정점 데이터.
     */
    std::vector<glm::vec3> vertices;

    /**
     * @brief 법선 데이터.
     */
    std::vector<glm::vec3> normals;

    /**
     * @brief 면 데이터.
     */
    std::vector<glm::uvec3> faces;
};

#endif // !GUARD_MODEL_H