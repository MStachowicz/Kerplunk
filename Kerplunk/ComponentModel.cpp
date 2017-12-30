#include "ComponentModel.h"



ComponentModel::ComponentModel(std::string &path, bool gamma) : IComponent(IComponent::ComponentFlags::COMPONENT_MODEL)
{
	model = Model(path, gamma);
}


ComponentModel::~ComponentModel()
{
}
