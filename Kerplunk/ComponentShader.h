#ifndef COMPONENTSHADER_H
#define COMPONENTSHADER_H

#include "IComponent.h"
#include "Shader.h"
#include <memory>
#include <glm/glm.hpp>

class ComponentShader : public IComponent
{
public:
	std::shared_ptr<Shader> shader;

	ComponentShader();
	ComponentShader(std::shared_ptr<Shader> pShader);

	~ComponentShader();
};
#endif