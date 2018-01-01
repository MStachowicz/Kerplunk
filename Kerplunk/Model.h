#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model
{
public:
	std::vector<MeshTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;


	// default constructor
	Model();
	// constructor, expects a filepath to a 3D model.
	Model(string const &path, bool gamma);
	// draws the model, and thus all its meshes binding 
	void Draw();
	// draws the model, and thus all its meshes
	void Draw(Shader shader);
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(std::string const &path);

private:
	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a MeshTexture struct.
	vector<MeshTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);
};

#endif