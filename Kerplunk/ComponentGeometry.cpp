#include "ComponentGeometry.h"

ComponentGeometry::ComponentGeometry(std::string filePath) : IComponent(IComponent::ComponentFlags::COMPONENT_GEOMETRY)
{
}

ComponentGeometry::ComponentGeometry(float pVertices[]) : IComponent(IComponent::ComponentFlags::COMPONENT_GEOMETRY)
{
}

ComponentGeometry::~ComponentGeometry() {}