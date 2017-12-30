#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <memory>
#include <string>
#include "Texture.h"
#include "Geometry.h"

static class ResourceManager
{
public:
	// directory of the project 
	std::string directory;

	// Container for all the pointers to the previously loaded geometry.
	static std::map<std::string, std::shared_ptr<Geometry>> geometryLibrary;
	static std::map<std::string, std::shared_ptr<Texture>> textureLibrary;

	ResourceManager();
	~ResourceManager();

	// Checks if the geometry has been previously loaded. Loads the geometry if previously loaded and 
	// creates new geometry and loads it if not.
	static std::shared_ptr<Geometry> LoadGeometry(std::string fileName);
	static std::shared_ptr<Geometry> LoadGeometry(Geometry::primitiveTypes primitive);
	static std::shared_ptr<Texture> LoadTexture(std::string &fileName, bool gammaCorrect);
};
#endif