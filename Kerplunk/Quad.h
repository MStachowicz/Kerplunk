#ifndef QUAD_H
#define QUAD_H

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

class Quad
{
public:
	unsigned int VAO_Handle;
	unsigned int VBO_Handle;
	std::vector<float> vertices;

	Quad();
	~Quad();

	void SetupQuad();
	void BufferData();
};
#endif