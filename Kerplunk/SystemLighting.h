#ifndef SYSTEMLIGHTING_H
#define SYSTEMLIGHTING_H

#include <iostream>
#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ISystem.h"
#include "Shader.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Geometry.h"

#include "ComponentRigidBody.h"
#include "ComponentDirectionalLight.h"
#include "ComponentSpotlight.h"
#include "ComponentPointLight.h"
#include "ComponentShadowCast.h"
#include "ComponentRenderable.h"

class SystemLighting : public ISystem
{
public:
	SystemLighting(std::shared_ptr<Camera> pCamera);
	~SystemLighting();

	// Screen dimensions
	const GLint SCR_WIDTH = 1920, SCR_HEIGHT = 1080;
	float viewDistanceFar = 2000.0f, viewDistanceNear = 0.1f;

	// Shader passed to all objects for rendering lights and objects in the scene.
	std::shared_ptr<Shader> lightingShader;
	// Shader to draw an always white geometry representing light source.
	Shader lightBoxShader;
	// The uniform buffer object containing the projection and view matrices of shaders.
	unsigned int ShaderUBO;
	// Geometric cube for drawing light boxes
	std::shared_ptr<Geometry> lightBox = ResourceManager::LoadGeometry("data/cube.txt");
	// Whether the lightBoxShader will be used to render light sources.
	bool drawLightSources = true;
	// Camera object of the scene.
	std::shared_ptr<Camera> camera;


	void OnLoad(const std::shared_ptr<Entity> &entity);
	void OnTickStart(const std::shared_ptr<Entity> &entity);
	void OnTickStart();
	void Tick(const std::shared_ptr<Entity> &entity);

	void SetupShader();
	void RenderLightBox(const glm::vec3 &pLightCubePosition);
};
#endif