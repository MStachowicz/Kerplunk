#include "ComponentGeometry.h"

ComponentGeometry::ComponentGeometry(std::string filePath) : IComponent(IComponent::ComponentFlags::COMPONENT_GEOMETRY)
{
	geometry = ResourceManager::LoadGeometry(filePath);
}

ComponentGeometry::~ComponentGeometry() {}