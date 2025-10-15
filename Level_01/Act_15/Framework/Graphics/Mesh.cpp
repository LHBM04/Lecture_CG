#include "Mesh.h"

#include <sstream>
#include <string>
#include <vector>

Mesh::Mesh(const char* source_) noexcept
{
    std::string source = {source_};

    std::vector<GLfloat> positions;
    std::vector<GLuint>  indices;

    // 데이터 크기 계산.
    {
        std::size_t vertexCount = 0;
        std::size_t faceCount   = 0;

        std::istringstream in(source);
        std::string line;

        while (std::getline(in, line))
        {
            if (line[0] == vertexPrefix && std::isspace(static_cast<unsigned char>(line[1])))
            {
                ++vertexCount;
            }
            else if (line[0] == facePrefix && std::isspace(static_cast<unsigned char>(line[1])))
            {
                ++faceCount;
            }
        }

        positions.reserve(vertexCount * 3);
        indices.reserve(faceCount * 3);
    }
    // 실제 파싱.
    {
        std::istringstream in(source);
        std::string line;

        while (std::getline(in, line))
        {
            std::istringstream ls(line.substr(2));

            if (line[0] == vertexPrefix && std::isspace(static_cast<unsigned char>(line[1])))
            {
                float x = 0.f, y = 0.f, z = 0.f;
                ls >> x >> y >> z;

                positions.push_back(x);
                positions.push_back(y);
                positions.push_back(z);
            }
            else if (line[0] == facePrefix && std::isspace(static_cast<unsigned char>(line[1])))
            {
                unsigned int v1, v2, v3;
                ls >> v1 >> v2 >> v3;

                indices.push_back(v1 - 1);
                indices.push_back(v2 - 1);
                indices.push_back(v3 - 1);
            }
        }
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(positions.size() * sizeof(GLfloat)), positions.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(3 * sizeof(float)), nullptr);
    glBindVertexArray(0);

    indexCount = static_cast<GLsizei>(indices.size());
}

Mesh::~Mesh() noexcept
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Mesh::Render() const noexcept
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
