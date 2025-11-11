#ifndef GUARD_MESH_H
#define GUARD_MESH_H

#include <glm/vec3.hpp>

class Mesh
{
public:
	struct Vertex final
	{ 
		glm::vec3 position;
		glm::vec3 color;
	};
};

#endif // !GUARD_MESH_H