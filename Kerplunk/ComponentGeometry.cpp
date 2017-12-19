#include "ComponentGeometry.h"


ComponentGeometry::ComponentGeometry(std::string filePath) : IComponent(IComponent::ComponentFlags::COMPONENT_GEOMETRY)
{
}

ComponentGeometry::ComponentGeometry(std::vector<float> pVertices) : IComponent(IComponent::ComponentFlags::COMPONENT_GEOMETRY)
{
	vertices = pVertices;
	numberOfTriangles = vertices.size() / 8; // 3 positions 3 normals 2 texcoords
	BufferData();
}

void ComponentGeometry::render()
{
	glBindVertexArray(VAO_Handle);
	glDrawArrays(GL_TRIANGLES, 0, numberOfTriangles);
}

void ComponentGeometry::BufferData()
{
	glGenVertexArrays(1, &VAO_Handle);
	glGenBuffers(1, &VBO_Handle);

	glBindVertexArray(VAO_Handle);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Handle);
	glBufferData(GL_ARRAY_BUFFER, vertices.size(), &vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind vao
	glBindVertexArray(0);
}

ComponentGeometry::~ComponentGeometry() {}