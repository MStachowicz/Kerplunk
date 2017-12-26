#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <math.h>

class IcoSphere
{
public:


	struct triangleindices
	{
		int v1, v2, v3;
		glm::vec3 faceNormal;

		triangleindices(int v1, int v2, int v3)
		{
			this->v1 = v1; this->v2 = v2; this->v3 = v3;
		}

		triangleindices(int v1, int v2, int v3, glm::vec3 pNormal)
		{
			this->v1 = v1;
			this->v2 = v2;
			this->v3 = v3;
			this->faceNormal = pNormal;
		}


	};
	struct vertex
	{
		// The position of the vertex
		glm::vec3 position;
		// The normal vector of this vertex averages between all the faces it belongs to
		glm::vec3 normal;
		// All the faces this vertex belongs to
		std::vector<std::shared_ptr<triangleindices>> faces;

		vertex(glm::vec3 &pPosition)
		{
			position = pPosition;
		}

		// sets the normal of the vertex by averaging the normal of all the faces it belongs to
		void setVertexNormal()
		{
			// ?Nv = ? p ?Nf where p is a weighting for each face which are already given in the faces.faceNormal, thus:
			// ?Nv = ? ?Nf
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				normal += faces[i]->faceNormal;
				normal = glm::normalize(normal);
			}
		}
	};

	IcoSphere() {}
	~IcoSphere() {}

	void Create(int recursionLevel)
	{
		middlePointIndexCache.clear();
		vertices.clear();
		indices.clear();
		faces.clear();
		index = 0;

		auto t = (1.0 + sqrt(5.0)) / 2.0;

		AddVertex(glm::vec3(-1, t, 0));
		AddVertex(glm::vec3(1, t, 0));
		AddVertex(glm::vec3(-1, -t, 0));
		AddVertex(glm::vec3(1, -t, 0));

		AddVertex(glm::vec3(0, -1, t));
		AddVertex(glm::vec3(0, 1, t));
		AddVertex(glm::vec3(0, -1, -t));
		AddVertex(glm::vec3(0, 1, -t));

		AddVertex(glm::vec3(t, 0, -1));
		AddVertex(glm::vec3(t, 0, 1));
		AddVertex(glm::vec3(-t, 0, -1));
		AddVertex(glm::vec3(-t, 0, 1));

		faces.push_back(std::make_shared<triangleindices>(0, 11, 5));
		faces.push_back(std::make_shared<triangleindices>(0, 5, 1));
		faces.push_back(std::make_shared<triangleindices>(0, 1, 7));
		faces.push_back(std::make_shared<triangleindices>(0, 7, 10));
		faces.push_back(std::make_shared<triangleindices>(0, 10, 11));

		faces.push_back(std::make_shared<triangleindices>(1, 5, 9));
		faces.push_back(std::make_shared<triangleindices>(5, 11, 4));
		faces.push_back(std::make_shared<triangleindices>(11, 10, 2));
		faces.push_back(std::make_shared<triangleindices>(10, 7, 6));
		faces.push_back(std::make_shared<triangleindices>(7, 1, 8));

		faces.push_back(std::make_shared<triangleindices>(3, 9, 4));
		faces.push_back(std::make_shared<triangleindices>(3, 4, 2));
		faces.push_back(std::make_shared<triangleindices>(3, 2, 6));
		faces.push_back(std::make_shared<triangleindices>(3, 6, 8));
		faces.push_back(std::make_shared<triangleindices>(3, 8, 9));

		faces.push_back(std::make_shared<triangleindices>(4, 9, 5));
		faces.push_back(std::make_shared<triangleindices>(2, 4, 11));
		faces.push_back(std::make_shared<triangleindices>(6, 2, 10));
		faces.push_back(std::make_shared<triangleindices>(8, 6, 7));
		faces.push_back(std::make_shared<triangleindices>(9, 8, 1));

		if (recursionLevel == 0) // if not generating extra faces set the normals of all the faces and add vertex info
		{
			for (int i = 0; i < faces.size(); i++)
				CalculateFaceNormal(i);
		}

		// Split the icohedron faces into 3 smaller faces to increase surface detail
		for (auto i = 0; i < recursionLevel; ++i)
		{
			auto faces2 = std::vector<std::shared_ptr<triangleindices>>();
			for (auto tri : faces)
			{
				int a = GetMiddlePoint(tri->v1, tri->v2);
				int b = GetMiddlePoint(tri->v2, tri->v3);
				int c = GetMiddlePoint(tri->v3, tri->v1);

				faces2.push_back(std::make_shared<triangleindices>(tri->v1, a, c));
				faces2.push_back(std::make_shared<triangleindices>(tri->v2, b, a));
				faces2.push_back(std::make_shared<triangleindices>(tri->v3, c, b));
				faces2.push_back(std::make_shared<triangleindices>(a, b, c));
			}

			// Rebuilding the faces vector using the new edge split version of the icohedron
			faces.clear();
			for (unsigned int j = 0; j < faces2.size(); ++j)
			{
				AddFace(faces2[j]);
				//faces.push_back(faces2[j]);
			}
		}

		for (int i = 0; i < vertices.size(); i++) // calculate the averaged vertex normals using the face normals
		{
			vertices[i].setVertexNormal();
		}

		for (auto tri : faces)
		{
			this->indices.push_back(tri->v1);
			this->indices.push_back(tri->v2);
			this->indices.push_back(tri->v3);
		}
	}

private:

	int AddVertex(glm::vec3 position)
	{
		double length = sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
		vertices.push_back(glm::vec3(position.x / length, position.y / length, position.z / length));

		return index++;
	}

	// Adds a face to the vector of triangle faces and sets the normal of the face while adding this face to the 3 vertices keeping track of which triangles they belong to
	void AddFace(std::shared_ptr<triangleindices> pFace)
	{
		faces.push_back(pFace);
		CalculateFaceNormal(faces.size() - 1);
	}

	// Calulates the face normal values and adds the face to the vector of faces its vertices are a member of.
	void CalculateFaceNormal(int facesIndex)
	{
		SetFaceNormal(faces[facesIndex]);
		setVertexFace(faces[facesIndex]);
	}	

	// sets the non unit vector face normal
	void SetFaceNormal(std::shared_ptr<triangleindices> face)
	{
		// find the normal of the face (not normalized maintaining the weighting of each face by its proportionality to the area of the triangle.
		//Nf = (?B - ?A) × (?C - ?A)
		face->faceNormal = glm::cross(glm::vec3(vertices[face->v2].position - vertices[face->v1].position), glm::vec3(vertices[face->v3].position - vertices[face->v1].position));
	}

	// Adds the face to the vector storing the faces each vertex belongs to.
	void setVertexFace(std::shared_ptr<triangleindices> face)
	{
		vertices[face->v1].faces.push_back(face);
		vertices[face->v2].faces.push_back(face);
		vertices[face->v3].faces.push_back(face);
	}

	int GetMiddlePoint(int p1, int p2)
	{
		bool firstPointIsSmaller = p1 < p2;
		int64_t smallerIndex = firstPointIsSmaller ? p1 : p2;
		int64_t greaterIndex = firstPointIsSmaller ? p2 : p1;
		int64_t key = (smallerIndex << 32) + greaterIndex;

		auto foundValueIterator = middlePointIndexCache.find(key);

		if (foundValueIterator != middlePointIndexCache.end())
		{
			return foundValueIterator->second;
		}
		else
		{
			glm::vec3 point1 = vertices[p1].position;
			glm::vec3 point2 = vertices[p2].position;
			glm::vec3 middle = glm::vec3((point1.x + point2.x) / 2.0,
				(point1.y + point2.y) / 2.0,
				(point1.z + point2.z) / 2.0);

			int i = this->AddVertex(middle);

			this->middlePointIndexCache.insert(std::make_pair(key, i));
			return i;
		}
	}

public:
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<triangleindices>> faces;

private:
	int index;
	std::map<int64_t, int> middlePointIndexCache;
};