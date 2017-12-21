#include "ComponentTexture.h"

ComponentTexture::ComponentTexture(std::string filePathDiffuse, bool gammeCorrect) : IComponent(IComponent::ComponentFlags::COMPONENT_TEXTURE)
{
	diffuseTexture = ResourceManager::LoadTexture(filePathDiffuse, gammeCorrect);
}

void ComponentTexture::AddSpecularTexture(std::string filePathSpecular)
{
	specularTexture = ResourceManager::LoadTexture(filePathSpecular, false);
}

void ComponentTexture::AddNormalMapTexture(std::string filePathNormalMap)
{
	normalMapTexture = ResourceManager::LoadTexture(filePathNormalMap, false);
}

ComponentTexture::~ComponentTexture() {}