#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include "Geometry.h"
#include <memory>

static class ResourceManager
{
public:
	// Container for all the pointers to the previously loaded geometry.
	static std::map<std::string, std::shared_ptr<Geometry>> geometryLibrary;

	// Checks if the geometry has been previously loaded. Loads the geometry if previously loaded and 
	// creates new geometry and loads it if not.
	static std::shared_ptr<Geometry> LoadGeometry(std::string fileName);
	~ResourceManager();
};
#endif