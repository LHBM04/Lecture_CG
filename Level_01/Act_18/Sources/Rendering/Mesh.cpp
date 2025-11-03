#include "Mesh.h"

#include <fstream>
#include <sstream>
#include <vector>

#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <spdlog/spdlog.h>

Mesh::Mesh(const std::vector<Vertex>& vertices_,
           const std::vector<GLuint>& indices_) noexcept
    : vao{0}
    , vbo{0}
    , ebo{0}
    , vertices{vertices_}
    , indices{indices_}
    , isInitialized{true}
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertex) * vertices.size(),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * indices.size(),
                 indices.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, texCoords)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, tangent)));

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    if (isInitialized)
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
}

void Mesh::Render() const noexcept
{
    if (!isInitialized)
    {
        return;
    }

    glBindVertexArray(vao);
    glDrawElements(GL_LINES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

Mesh* Mesh::LoadFrom(const std::string& filePath_) noexcept
{
    std::ifstream ifs(filePath_);
    if (!ifs.is_open())
    {
        spdlog::error("OBJ open failed: {}", filePath_);
        return nullptr;
    }

    std::vector<glm::vec3>        positions;
    std::vector<std::vector<int>> faces;
    std::vector<std::vector<int>> lines;

    std::string line;
    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tag;
        iss >> tag;

        if (tag == "v")
        {
            glm::vec3 p; iss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        }
        else if (tag == "f")
        {
            std::vector<int> face_indices;
            std::string token;
            while (iss >> token)
            {
                size_t slash = token.find('/');
                std::string vi = (slash == std::string::npos) ? token : token.substr(0, slash);
                if (!vi.empty())
                {
                    int idx = std::stoi(vi);
                    face_indices.push_back(idx);
                }
            }
            if (face_indices.size() >= 3)
                faces.push_back(std::move(face_indices));
        }
        else if (tag == "l") // <-- [추가 시작]
        {
            std::vector<int> line_indices;
            std::string token;
            while (iss >> token)
            {
                // l v1 v2 v3 ... 형식 지원 (v1/vt/vn 무시)
                std::string vi = token.substr(0, token.find('/'));
                if (!vi.empty())
                {
                    line_indices.push_back(std::stoi(vi));
                }
            }
            if (line_indices.size() >= 2)
            {
                lines.push_back(std::move(line_indices));
            }
        }
    }

    if (positions.empty())
    {
        spdlog::error("OBJ has no positions: {}", filePath_);
        return nullptr;
    }

    std::vector<Vertex> out_vertices(positions.size());
    std::vector<GLuint> out_indices;

    for (size_t i = 0; i < positions.size(); ++i)
    {
        out_vertices[i].position = positions[i];
        out_vertices[i].normal = glm::vec3(0.0f);
    }

    for (const auto& face : faces)
    {
        for (size_t i = 1; i + 1 < face.size(); ++i)
        {
            int a_idx = face[0] - 1;
            int b_idx = face[i] - 1;
            int c_idx = face[i+1] - 1;

            if (a_idx < 0 || b_idx < 0 || c_idx < 0 ||
                a_idx >= static_cast<int>(positions.size()) || b_idx >= static_cast<int>(positions.size()) || c_idx >= (int)positions.size())
            {
                continue;
            }

            out_indices.push_back(static_cast<GLuint>(a_idx));
            out_indices.push_back(static_cast<GLuint>(b_idx));
            out_indices.push_back(static_cast<GLuint>(c_idx));

            glm::vec3 p0 = positions[a_idx];
            glm::vec3 p1 = positions[b_idx];
            glm::vec3 p2 = positions[c_idx];
            glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

            if (!glm::any(glm::isnan(normal)))
            {
                out_vertices[a_idx].normal += normal;
                out_vertices[b_idx].normal += normal;
                out_vertices[c_idx].normal += normal;
            }
        }
    }

    for (const auto& line_segment : lines)
    {
        for (size_t i = 0; i + 1 < line_segment.size(); ++i)
        {
            int a_idx = line_segment[i] - 1;
            int b_idx = line_segment[i + 1] - 1;

            if (a_idx < 0 || b_idx < 0 ||
                a_idx >= static_cast<int>(positions.size()) ||
                b_idx >= static_cast<int>(positions.size()))
            {
                continue;
            }

            out_indices.push_back(static_cast<GLuint>(a_idx));
            out_indices.push_back(static_cast<GLuint>(b_idx));
        }
    }

    for (auto &v : out_vertices)
    {
        if (glm::length(v.normal) > 1e-6f)
        {
            v.normal = glm::normalize(v.normal);
        }
        else
        {
            v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        }
    }

    return new Mesh(out_vertices, out_indices);
}