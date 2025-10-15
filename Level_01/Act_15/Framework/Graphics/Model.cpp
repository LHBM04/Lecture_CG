#include "Model.h"

#include <sstream>
#include <string>

#include <spdlog/spdlog.h>

#include "../Misc/File.h"

Model::Model(const char* const source_) noexcept
{
    std::string source = {source_};

    std::istringstream in(source);
    std::string line;

    while (std::getline(in, line))
    {
        if (line.substr(0, 2) == vertexPrefix)
        {
            std::istringstream ls(line.substr(2));
            glm::vec3 vertex;
            ls >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (line.substr(0, 2) == normalPrefix)
        {
            std::istringstream ls(line.substr(3));
            glm::vec3 normal;
            ls >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (line.substr(0, 2) == facePrefix)
        {
            std::istringstream ls(line.substr(2));
            std::string vertex_str;
            unsigned int v, n;
            char slash;

            for (int i = 0; i < 3; ++i)
            {
                ls >> v >> slash >> slash >> n;
                faces.emplace_back(v - 1, 0, n - 1); // vt is not used, so it's 0.
            }
        }
    }
}