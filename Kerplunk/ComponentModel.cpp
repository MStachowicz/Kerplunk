#include "ComponentModel.h"

ComponentModel::ComponentModel(std::string &path, bool gamma) : IComponent(IComponent::ComponentFlags::COMPONENT_MODEL)
{
	model = ResourceManager::LoadModel(path, gamma);
}


ComponentModel::~ComponentModel()
{
}
