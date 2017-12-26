#include "Geometry.h"

Geometry::Geometry() {}

std::string Geometry::ToString(primitiveTypes type)
{
	switch (type)
	{
	case Geometry::icoSphere: return "icoSphere";

	default: return "ERROR primitive type not implemented in geometry";
	}
}

void Geometry::LoadGeometry(std::string pFilePath)
{
	std::ifstream file(pFilePath);
	std::string line;

	while (std::getline(file, line))
	{
		float i;

		std::stringstream linestream(line);
		while (linestream >> i)
		{
			vertices.push_back(i);

			if (linestream.peek() == ',' || linestream.peek() == ' ')
				linestream.ignore();
		}
	}

	numberOfTriangles = vertices.size() / 8;
	BufferData();
}


void Geometry::generateIcoSphere()
{
	IcoSphere sphere;
	sphere.Create(4);

	// Converting indexed vertex information into flat vector of vertex positions and normals to set the attribute pointer in shader correctly.
	for (int i = 0; i < sphere.indices.size(); i++)
	{
		// Positions
		vertices.push_back(sphere.vertices[sphere.indices[i]].position.x);
		vertices.push_back(sphere.vertices[sphere.indices[i]].position.y);
		vertices.push_back(sphere.vertices[sphere.indices[i]].position.z);
		// Normals
		vertices.push_back(sphere.vertices[sphere.indices[i]].normal.x);
		vertices.push_back(sphere.vertices[sphere.indices[i]].normal.y);
		vertices.push_back(sphere.vertices[sphere.indices[i]].normal.z);
		// texture coords (not implemented)
		vertices.push_back(0);
		vertices.push_back(0);
	}

	numberOfTriangles = vertices.size() / 8;
	BufferData();
}

void Geometry::LoadGeometry(primitiveTypes type)
{
	switch (type)
	{
	case Geometry::icoSphere:
		Geometry::generateIcoSphere();
		break;
	default:
		"ERROR primitive type not implemented in geometry";
		break;
	}
}

void Geometry::render()
{
	glBindVertexArray(VAO_Handle);
	glDrawArrays(GL_TRIANGLES, 0, numberOfTriangles);
	glBindVertexArray(0);
}

void Geometry::BufferData()
{
	glGenVertexArrays(1, &VAO_Handle);
	glGenBuffers(1, &VBO_Handle);

	glBindVertexArray(VAO_Handle);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Handle);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

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

void Geometry::renderElements()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Handle);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Geometry::BufferElementData()
{
	glGenBuffers(1, &EBO_Handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_Handle);
	glGenBuffers(1, &VBO_Handle);
	glGenBuffers(1, &EBO_Handle);

	glBindVertexArray(VAO_Handle);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Handle);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Geometry::~Geometry() {}