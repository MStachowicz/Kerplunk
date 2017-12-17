#include "ISystem.h"

//void ISystem::OnAction(Entity &entity) {}

ISystem::ISystem(std::string pName, IComponent::ComponentFlags MASK) : name(pName), MASK(MASK)
{}

ISystem::~ISystem() {}
