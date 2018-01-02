#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <map>
#include <string>

#include "IcoSphere.h"

class Geometry
{
public:
	enum AttributeTypes {
		Position,
		Normal,
		Texture,
		Tangent,
		Bitangent
	};
	enum primitiveTypes {
		icoSphere,
	};

	struct Attribute
	{
		AttributeTypes attributeType;
		int shaderLocation; // location of the attribute in the shader
		int size; // how many values the attribute consists of
		bool isNormalized; // are the values normalized
		int offset; // how many values along in the vertices vector is the first value
		GLenum dataType; // the type of the values stored

		Attribute(AttributeTypes pType, int pOffset) : attributeType(pType), offset(pOffset)
		{
			switch (pType)
			{
			case AttributeTypes::Position:
				shaderLocation = 0;
				size = 3;
				isNormalized = false;
				dataType = GL_FLOAT;
				break;
			case AttributeTypes::Normal:
				shaderLocation = 1;
				size = 3;
				isNormalized = true;
				dataType = GL_FLOAT;
				break;
			case AttributeTypes::Texture:
				shaderLocation = 2;
				size = 2;
				isNormalized = false;
				dataType = GL_FLOAT;
				break;
			case AttributeTypes::Tangent:
				shaderLocation = 3;
				size = 3;
				isNormalized = false;
				dataType = GL_FLOAT;
				break;
			case AttributeTypes::Bitangent:
				shaderLocation = 4;
				size = 3;
				isNormalized = false;
				dataType = GL_FLOAT;
				break;
			}
		}
	};
	static std::string ToString(primitiveTypes type);

	std::vector<Attribute> attributes; // all the attributes types stored in the vertices vector.
	std::vector<float> vertices; // Stores all the vertex data associated with the object. Order = Position, Normal, texcoord, tangent, bitangent
	std::vector<unsigned int> indices;
	unsigned int numberOfTriangles = 0;
	unsigned int VAO_Handle = 0;
	unsigned int VBO_Handle = 0;
	unsigned int EBO_Handle = 0;


	// Create a geometry oject loading in vertex data from a file.
	Geometry();

	void LoadGeometry(std::string pFilePath);
	void LoadGeometry(primitiveTypes type);

	void render();
	void renderElements();

	~Geometry();

private:
	int stride = 0;

	void BufferData();
	void BufferData(int numAttributes);

	void BufferElementData();
	void generateIcoSphere();
};
#endif