#include "ComponentGeometry.h"

ComponentGeometry::ComponentGeometry(std::string filePath) : IComponent(IComponent::ComponentFlags::COMPONENT_GEOMETRY)
{
	geometry = ResourceManager::LoadGeometry(filePath);
}

ComponentGeometry::ComponentGeometry(Geometry::primitiveTypes primitive) : IComponent(IComponent::ComponentFlags::COMPONENT_GEOMETRY)
{
	geometry = ResourceManager::LoadGeometry(primitive);
}

ComponentGeometry::~ComponentGeometry() {}