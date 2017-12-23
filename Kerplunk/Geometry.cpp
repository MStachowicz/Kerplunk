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



Geometry::~Geometry() {}