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
        reinterpret_cast<void*>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, texCoord)));

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

    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;

    std::vector<Vertex> outVertices;
    std::vector<GLuint> outIndices;

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
            tempPositions.push_back(p);
        }
        else if (tag == "vn")
        {
            glm::vec3 n;
            iss >> n.x >> n.y >> n.z;
            tempNormals.push_back(n);
        }
        else if (tag == "vt") // 텍스쳐 좌표 파싱 추가
        {
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            tempTexCoords.push_back(uv);
        }
        else if (tag == "f")
        {
            std::vector<std::string> faceTokens;
            std::string token;
            while (iss >> token)
            {
                faceTokens.push_back(token);
            }

            for (std::size_t i = 1; i + 1 < faceTokens.size(); ++i)
            {
                const std::string* triTokens[3] = { &faceTokens[0], &faceTokens[i], &faceTokens[i + 1] };

                for (const auto* t : triTokens)
                {
                    std::stringstream ss(*t);
                    std::string segment;
                    std::vector<std::string> indices;

                    while (std::getline(ss, segment, '/'))
                    {
                        indices.push_back(segment);
                    }

                    Vertex vertex{};

                    if (!indices.empty() && !indices[0].empty())
                    {
                        int posIdx = std::stoi(indices[0]) - 1;
                        vertex.position = tempPositions[posIdx];
                    }
                    if (indices.size() >= 2 && !indices[1].empty())
                    {
                        int uvIdx = std::stoi(indices[1]) - 1;
                        vertex.texCoord = tempTexCoords[uvIdx];
                    }
                    if (indices.size() >= 3 && !indices[2].empty())
                    {
                        int normIdx = std::stoi(indices[2]) - 1;
                        vertex.normal = tempNormals[normIdx];
                    }
                    else
                    {
                        vertex.normal = glm::normalize(vertex.position);
                    }

                    outVertices.push_back(vertex);
                    outIndices.push_back(static_cast<GLuint>(outVertices.size() - 1));
                }
            }
        }
    }

    if (outVertices.empty())
    {
        std::println("OBJ load failed or empty: {}", filePath_);
        return nullptr;
    }
     
    return std::make_unique<Mesh>(outVertices, outIndices);
}