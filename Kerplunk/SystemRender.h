#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include "ISystem.h"
#include "IComponent.h"
#include "Shader.h"
#include "ComponentShadowCast.h"
#include "ComponentRenderable.h"
#include "ComponentTexture.h"
#include "ComponentMaterial.h"
#include "ComponentModel.h"
#include "ComponentGeometry.h"
#include "ComponentRigidBody.h"
#include <memory>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ComponentDirectionalLight.h"

class SystemRender : public ISystem
{
public:
	SystemRender();
	~SystemRender();

	const GLint SCR_WIDTH = 1920, SCR_HEIGHT = 1080; // Screen dimensions.

	void OnLoad(const std::shared_ptr<Entity> &entity);
	void OnTickStart(const std::shared_ptr<Entity> &entity);
	void OnTickStart();
	void Tick(const std::shared_ptr<Entity> &entity);
};
#endif