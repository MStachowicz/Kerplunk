#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include "ISystem.h"
#include "IComponent.h"
#include "Shader.h"
#include "ComponentPosition.h"
#include "ComponentRotation.h"
#include "ComponentScale.h"
#include "ComponentTexture.h"
#include "ComponentGeometry.h"
#include "ComponentShader.h"
#include <memory>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SystemRender : public ISystem
{
public:
	SystemRender();
	void OnAction(Entity &entity);
	~SystemRender();
};
#endif