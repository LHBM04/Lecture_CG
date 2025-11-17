#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices_,
           const std::vector<GLuint>& indices_) noexcept
    : vao(0)
    , vbo(0)
    , ebo(0)
    , vertices(vertices_)
    , indices(indices_)
    , isInitialized(true)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(Vertex) * vertices.size(),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * indices.size(),
        indices.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, color)));

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

void Mesh::Render(const GLenum renderMode_) const noexcept
{
    if (!isInitialized)
    {
        return;
    }

    glBindVertexArray(vao);
    glDrawElements(renderMode_, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

std::unique_ptr<Mesh> Mesh::LoadFrom(const std::string& filePath_) noexcept
{
    std::ifstream ifs(filePath_);
    if (!ifs.is_open())
    {
        std::println("OBJ open failed: {}", filePath_);
        return nullptr;
    }

    std::vector<glm::vec3>        positions;
    std::vector<glm::vec3>        colors;
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
            glm::vec3 p;
            iss >> p.x >> p.y >> p.z;
            positions.push_back(p);

            glm::vec3 c;
            iss >> c.r >> c.g >> c.b;
            colors.push_back(c);
        }
        else if (tag == "f")
        {
            std::vector<int> face_indices;
            std::string token;
            while (iss >> token)
            {
                std::size_t slash = token.find('/');
                std::string vi = (slash == std::string::npos) ? token : token.substr(0, slash);
                if (!vi.empty())
                {
                    int idx = std::stoi(vi);
                    face_indices.push_back(idx);
                }
            }
            if (face_indices.size() >= 3)
            {
                faces.push_back(std::move(face_indices));
            }
        }
        else if (tag == "l")
        {
            std::vector<int> line_indices;
            std::string token;
            while (iss >> token)
            {
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
        std::println("OBJ has no positions: {}", filePath_);
        return nullptr;
    }

    std::vector<Vertex> outVertices(positions.size());
    std::vector<GLuint> outIndices;

    for (std::size_t i = 0; i < positions.size(); ++i)
    {
        outVertices[i].position = positions[i];
        outVertices[i].color    = colors[i];
    }

    for (const auto& face : faces)
    {
        for (std::size_t i = 1; i + 1 < face.size(); ++i)
        {
            int a_idx = face[0] - 1;
            int b_idx = face[i] - 1;
            int c_idx = face[i + 1] - 1;

            if (a_idx < 0 || b_idx < 0 || c_idx < 0 ||
                a_idx >= static_cast<int>(positions.size()) ||
                b_idx >= static_cast<int>(positions.size()) ||
                c_idx >= static_cast<int>(positions.size()))
            {
                continue;
            }

            outIndices.push_back(static_cast<GLuint>(a_idx));
            outIndices.push_back(static_cast<GLuint>(b_idx));
            outIndices.push_back(static_cast<GLuint>(c_idx));
        }
    }

    for (const auto& line_segment : lines)
    {
        for (std::size_t i = 0; i + 1 < line_segment.size(); ++i)
        {
            int a_idx = line_segment[i] - 1;
            int b_idx = line_segment[i + 1] - 1;

            if (a_idx < 0 || b_idx < 0 ||
                a_idx >= static_cast<int>(positions.size()) ||
                b_idx >= static_cast<int>(positions.size()))
            {
                continue;
            }

            outIndices.push_back(static_cast<GLuint>(a_idx));
            outIndices.push_back(static_cast<GLuint>(b_idx));
        }
    }

    return std::make_unique<Mesh>(outVertices, outIndices);
}