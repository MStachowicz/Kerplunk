#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include "ISystem.h"
#include "IComponent.h"
#include "Shader.h"
#include "ComponentPosition.h"
#include "ComponentGeometry.h"
#include "ComponentShader.h"
#include <memory>
#include <glm\glm.hpp>

class SystemRender : public ISystem
{
public:
	SystemRender();
	void OnAction(Entity &entity);
	// Sets the entity model matrix in the shader
	void setShaderModel(std::shared_ptr<Shader> shader, glm::vec3 &position, glm::vec3 &rotation, glm::vec3 &scale);
	~SystemRender();
};
#endif