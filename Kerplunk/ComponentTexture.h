#ifndef COMPONENTTEXTURE_H
#define COMPONENTTEXTURE_H

#include <memory>
#include <string>
#include "IComponent.h"
#include "Texture.h"
#include "ResourceManager.h"

class ComponentTexture : public IComponent
{
public:
	std::shared_ptr <Texture> diffuseTexture;
	std::shared_ptr <Texture> specularTexture;
	std::shared_ptr <Texture> normalMapTexture;

	ComponentTexture(std::string filePathDiffuse, bool gammaCorrect);
	void AddSpecularTexture(std::string filePathSpecular);
	void AddNormalMapTexture(std::string filePathNormalMap);
	~ComponentTexture();
};
#endif