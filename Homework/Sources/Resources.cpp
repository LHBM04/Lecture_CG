#include "Resources.h"

#include "Mesh.h"

const std::string& Resources::GetShader(std::string_view path_) noexcept
{
	if (!shaders.contains(path_))
	{
		LoadShader(path_);
	}

	return shaders[path_];
}

Mesh* const Resources::GetMesh(std::string_view path_) noexcept
{
	if (!meshes.contains(path_))
	{
		LoadMesh(path_);
	}

	return meshes[path_].get();
}

void Resources::LoadShader(std::string_view path_) noexcept
{
	if (shaders.contains(path_))
	{
		return;
	}

	std::ifstream fileStream(path_.data());
	if (!fileStream.is_open())
	{
        spdlog::error("Shader open failed: {}\n", path_);
		return;
	}
	
    std::stringstream buffer;
	buffer << fileStream.rdbuf();
	
    shaders[path_] = buffer.str();
}
 
void Resources::LoadMesh(std::string_view path_) noexcept
{
    std::ifstream ifs(path_.data());
    if (not ifs)
    {
        spdlog::error("OBJ open failed: {}", path_);
        return;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_colors; 
    std::vector<glm::vec3> temp_normals;

    std::vector<Mesh::Vertex> finalVertices;
    std::vector<GLuint>       finalIndices;

    std::string line;
    while (std::getline(ifs, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::istringstream iss(line);
        std::string        tag;
        iss >> tag;

        if (tag == "v")
        {
            glm::vec3 p;
            iss >> p.x >> p.y >> p.z;
            temp_positions.push_back(p);

            glm::vec3 c(1.0f);
            if (iss >> c.r >> c.g >> c.b) 
            {
                temp_colors.push_back(c);
            }
            else 
            {
                temp_colors.push_back(glm::vec3(1.0f));
            }
        }
        else if (tag == "vn")
        {
            glm::vec3 n;
            iss >> n.x >> n.y >> n.z;
            temp_normals.push_back(n);
        }
        else if (tag == "f")
        {
            std::vector<Mesh::Vertex> faceVertices;
            std::string               token;

            while (iss >> token)
            {
                int vIdx = -1;
                int nIdx = -1;

                std::size_t firstSlash = token.find('/');
                std::size_t secondSlash = token.find('/', firstSlash + 1);

                std::string vStr = token.substr(0, firstSlash);
                vIdx = std::stoi(vStr) - 1;

                if (secondSlash != std::string::npos)
                {
                    std::string nStr = token.substr(secondSlash + 1);
                    if (!nStr.empty())
                    {
                        nIdx = std::stoi(nStr) - 1;
                    }
                }

                Mesh::Vertex currentVertex;

                if (vIdx >= 0 && vIdx < temp_positions.size())
                {
                    currentVertex.position = temp_positions[vIdx];

                    if (vIdx < temp_colors.size()) 
                    {
                        currentVertex.color = temp_colors[vIdx];
                    }
                    else 
                    {
                        currentVertex.color = glm::vec3(1.0f);
                    }
                }

                if (nIdx >= 0 && nIdx < temp_normals.size())
                {
                    currentVertex.normal = temp_normals[nIdx];
                }
                else
                {
                    currentVertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                }

                faceVertices.push_back(currentVertex);
            }

            for (size_t i = 1; i + 1 < faceVertices.size(); ++i)
            {
                finalVertices.push_back(faceVertices[0]);
                finalIndices.push_back(static_cast<GLuint>(finalVertices.size() - 1));

                finalVertices.push_back(faceVertices[i]);
                finalIndices.push_back(static_cast<GLuint>(finalVertices.size() - 1));

                finalVertices.push_back(faceVertices[i + 1]);
                finalIndices.push_back(static_cast<GLuint>(finalVertices.size() - 1));
            }
        }
    }

    if (finalVertices.empty())
    {
        spdlog::error("OBJ load failed or empty: {}", path_);
        return;
    }

    meshes[path_] = std::make_unique<Mesh>(finalVertices, finalIndices);
}

std::unordered_map<std::filesystem::path, std::string> Resources::shaders = {};

std::unordered_map<std::filesystem::path, std::unique_ptr<Mesh>> Resources::meshes = {};