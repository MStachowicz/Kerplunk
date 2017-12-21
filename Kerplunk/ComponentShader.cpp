#include "ComponentShader.h"

ComponentShader::ComponentShader() : IComponent(IComponent::ComponentFlags::COMPONENT_SHADER) {}

ComponentShader::ComponentShader(std::shared_ptr<Shader> pShader) : IComponent(IComponent::ComponentFlags::COMPONENT_SHADER)
{
	shader = pShader;
}

ComponentShader::~ComponentShader() {}