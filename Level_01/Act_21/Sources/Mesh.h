#ifndef GUARD_MESH_H
#define GUARD_MESH_H

#include <string>

#include <GL/glew.h>

#include <glm/vec3.hpp>

/**
 * @brief 메쉬를 정의합니다.
 */
class Mesh
{
public:
	/**
	 * @brief 정점을 정의합니다.
	 */
	struct Vertex final
	{ 
		/**
		 * @brief 정점 위치.
		 */
		glm::vec3 position;

		/**
		 * @brief 정점 색.
		 */
		glm::vec3 color;
	};

    /**
     * @brief 생성자.
     *
     * @param vertices_ 생성할 정점들.
     * @param indices_  생성할 인덱스들.
     */
    explicit Mesh(
        const std::vector<Vertex>& vertices_,
        const std::vector<GLuint>& indices_
    ) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Mesh();

    /**
     * @brief 해당 메쉬를 렌더링합니다.
     */
    void Render() const noexcept;

    /**
     * @brief 파일에서 메쉬를 로드합니다.
     *
     * @param filePath_ 로드할 파일 경로.
     *
     * @return Mesh* 로드된 메쉬 객체.
     */
    [[nodiscard]]
    static Mesh* LoadFrom(const std::string& filePath_) noexcept;

private:
    /**
     * @brief 해당 정점 배열 객체.
     */
    unsigned int vao;

    /**
     * @brief 해당 매쉬의 정점 버퍼 객체.
     */
    unsigned int vbo;

    /**
     * @brief 해당 매쉬의 요소 배열 객체.
     */
    unsigned int ebo;

    /**
     * @brief 해당 매쉬의 정점 데이터들.
     */
    std::vector<Vertex> vertices;

    /**
     * @brief 해당 매쉬의 인덱스들.
     */
    std::vector<GLuint> indices;

    /**
     * @brief 해당 메쉬의 초기화 여부.
     */
    bool isInitialized;
};

#endif // !GUARD_MESH_H