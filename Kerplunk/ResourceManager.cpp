#include "ResourceManager.h"

std::map<std::string, std::shared_ptr<Geometry>> ResourceManager::geometryLibrary;
std::map<std::string, std::shared_ptr<Texture>> ResourceManager::textureLibrary;




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

std::shared_ptr<Geometry> ResourceManager::LoadGeometry(Geometry::primitiveTypes primitive)
{
	// stores the name of the primitive in a sring to place into the geometry library
	std::string primitiveFilename = Geometry::ToString(primitive);
	auto it = ResourceManager::geometryLibrary.find(primitiveFilename);

	if (it != ResourceManager::geometryLibrary.end()) // element found, returns the pointer to the element
	{
		return it->second;
	}
	else // if not found, create and load the geometry then add it to the map
	{
		std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();;
		/*geometryLibrary.insert(std::pair<std::string, std::shared_ptr<Geometry>>
		(fileName, geometry));*/
		geometryLibrary.emplace(std::pair<std::string, std::shared_ptr<Geometry>>
			(primitiveFilename, geometry));
		geometry->LoadGeometry(primitive);
		return geometry;
	}
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(std::string &fileName, bool gammeCorrect)
{
	auto it = ResourceManager::textureLibrary.find(fileName);

	if (it != ResourceManager::textureLibrary.end()) // element found, returns the pointer to the element
	{
		return it->second;
	}
	else // if not found, create and load the texture then add it to the map
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();;
		/*geometryLibrary.insert(std::pair<std::string, std::shared_ptr<Geometry>>
		(fileName, geometry));*/
		
		textureLibrary.emplace(std::pair<std::string, std::shared_ptr<Texture>>
			(fileName, texture));
		texture->LoadTexture(fileName, gammeCorrect);
		return texture;
	}


	return std::shared_ptr<Texture>();
}

ResourceManager::ResourceManager()
{
	// sets the path of the file storing resources
	//directory = path.substr(0, path.find_last_of('/'));
}
ResourceManager::~ResourceManager()
{
}
