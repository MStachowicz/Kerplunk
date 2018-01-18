#include "ISystem.h"

ISystem::ISystem(std::string pName, IComponent::ComponentFlags MASK) : name(pName), MASK(MASK)
{}

ISystem::~ISystem() {}
