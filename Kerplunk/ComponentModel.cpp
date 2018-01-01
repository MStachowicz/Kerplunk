#include "ComponentModel.h"

ComponentModel::ComponentModel(std::string &path, bool gamma, bool useOriginalTextures) : IComponent(IComponent::ComponentFlags::COMPONENT_MODEL), 
useModelTextures(useOriginalTextures)
{
	model = ResourceManager::LoadModel(path, gamma);
}

ComponentModel::~ComponentModel() {}