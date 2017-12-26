#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial() : IComponent(IComponent::ComponentFlags::COMPONENT_MATERIAL)
{
}

ComponentMaterial::ComponentMaterial(glm::vec3 pAmbientDiffuse) : IComponent(IComponent::ComponentFlags::COMPONENT_MATERIAL), 
ambient(pAmbientDiffuse), diffuse(pAmbientDiffuse)
{
}

ComponentMaterial::ComponentMaterial(glm::vec3 pAmbientDiffuse, glm::vec3 pSpecular) : IComponent(IComponent::ComponentFlags::COMPONENT_MATERIAL), 
ambient(pAmbientDiffuse), diffuse(pAmbientDiffuse), specular(pSpecular), shininess(DEFAULTSHININESS)
{
}

ComponentMaterial::ComponentMaterial(glm::vec3 pAmbientDiffuse, glm::vec3 pSpecular, float pShininess) : IComponent(IComponent::ComponentFlags::COMPONENT_MATERIAL),
ambient(pAmbientDiffuse), diffuse(pAmbientDiffuse), specular(pSpecular), shininess(pShininess)
{
}

ComponentMaterial::ComponentMaterial(glm::vec3 pAmbient, glm::vec3 pDiffuse, glm::vec3 pSpecular, float pShininess) : IComponent(IComponent::ComponentFlags::COMPONENT_MATERIAL),
ambient(pAmbient), diffuse(pDiffuse), specular(pSpecular), shininess(pShininess)
{
}

ComponentMaterial::~ComponentMaterial() {}