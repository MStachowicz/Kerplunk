#include "ResourceManager.h"

std::map<std::string, std::shared_ptr<Geometry>> ResourceManager::geometryLibrary;

std::shared_ptr<Geometry> ResourceManager::LoadGeometry(std::string fileName)
{
	auto it = ResourceManager::geometryLibrary.find(fileName);

	if (it != ResourceManager::geometryLibrary.end()) // element found, returns the pointer to the element
	{
		return it->second;
	}
	else // if not found, create and load the geometry then add it to the map
	{
		std::shared_ptr<Geometry> geometry; 
		geometry->LoadGeometry(fileName);
		geometryLibrary.insert(std::pair<std::string, std::shared_ptr<Geometry>>
			(fileName, geometry));
		return geometry;
	}
}


ResourceManager::~ResourceManager()
{
}
