#include "ResourceManager.h"

std::map<std::string, std::shared_ptr<Geometry>> ResourceManager::geometryLibrary;
std::map<std::string, std::shared_ptr<Texture>> ResourceManager::textureLibrary;
std::map<std::string, std::shared_ptr<Model>> ResourceManager::modelLibrary;

std::string ResourceManager::directory = ResourceManager::SetDirectory();

// Returns the index of the nth occurence of a string from the end of the string
int nthOccurrence(const std::string& str, const std::string& findMe, int nth)
{
	size_t  pos = str.length();
	int     cnt = 0;

	while (cnt != nth)
	{
		pos -= 1;
		pos = str.rfind(findMe, pos);

		if (pos == std::string::npos)
			return -1;

		cnt++;
	}

	return pos;
}

// Replaces all the occurrences of a string in a string with a new string. string.
void replaceAllOccurrrences(string& baseStr, const string& strToReplace, const string& replaceString)
{
	unsigned int pos = 0;

	// While the string to replace is present in the base string.
	while (baseStr.find(strToReplace, 0) != std::string::npos)
	{
		pos++;
		pos = baseStr.find(strToReplace, pos);

		if (pos == std::string::npos)
			break;

		baseStr.replace(pos, 1, replaceString);
	}
}

// Sets the path to the directory of the resource folder in the solution.
std::string ResourceManager::SetDirectory()
{
	// Find the full path to the exe
	char result[MAX_PATH];
	string(result, GetModuleFileName(NULL, result, MAX_PATH));
	string path = (string)result;

	// Find the second to last occurence of the Kerplunk directory
	string dir = "Kerplunk"; // the directory the path will be trimmed after
	int dirPos = nthOccurrence(path, dir, 2);
	// Cut down the path to the directory of the solution
	path = path.substr(0, dirPos + dir.length());
	// Add the directory of the resource files
	path.append("/Kerplunk/Resources/");
	// Replace all the double backslashes with forward ones.
	replaceAllOccurrrences(path, "\\", "/");

	return path;
}



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

		std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();
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
		std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();
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
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		/*geometryLibrary.insert(std::pair<std::string, std::shared_ptr<Geometry>>
		(fileName, geometry));*/

		textureLibrary.emplace(std::pair<std::string, std::shared_ptr<Texture>>
			(fileName, texture));
		texture->LoadTexture(fileName, gammeCorrect);
		return texture;
	}
}

std::shared_ptr<Model> ResourceManager::LoadModel(std::string & fileName, bool gammaCorrect)
{
	auto it = ResourceManager::modelLibrary.find(fileName);

	if (it != ResourceManager::modelLibrary.end()) // element found, returns the pointer to the element
	{
		return it->second;
	}
	else // if not found, create and load the texture then add it to the map
	{
		std::shared_ptr<Model> model = std::make_shared<Model>();
		/*geometryLibrary.insert(std::pair<std::string, std::shared_ptr<Geometry>>
		(fileName, geometry));*/

		modelLibrary.emplace(std::pair<std::string, std::shared_ptr<Model>>
			(fileName, model));
		model->gammaCorrection = gammaCorrect;
		model->loadModel(fileName);
		return model;
	}
}

ResourceManager::~ResourceManager() {}