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
		//Geometry geometry;

		std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();;
		/*geometryLibrary.insert(std::pair<std::string, std::shared_ptr<Geometry>>
			(fileName, geometry));*/
		geometryLibrary.emplace(std::pair<std::string, std::shared_ptr<Geometry>>
			(fileName, geometry));
		geometry->LoadGeometry(fileName);
		return geometry;
	}
}


ResourceManager::~ResourceManager()
{
}
