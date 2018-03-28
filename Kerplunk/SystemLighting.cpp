#include "SystemLighting.h"

SystemLighting::SystemLighting(std::shared_ptr<Camera> pCamera) :
	ISystem("SystemLighting", (IComponent::ComponentFlags)(IComponent::COMPONENT_DIRECTIONAL_LIGHT | IComponent::COMPONENT_POINTLIGHT | IComponent::COMPONENT_SPOTLIGHT)),
	lightingShader(std::make_shared<Shader>("../Kerplunk/lighting.vert", "../Kerplunk/lighting.frag", "../Kerplunk/explode.geom")),
	lightBoxShader("../Kerplunk/lightBox.vert", "../Kerplunk/lightBox.frag", nullptr),
	camera(pCamera)
{
	SetupShader();
}
SystemLighting::~SystemLighting()
{
}

void SystemLighting::OnLoad(const std::shared_ptr<Entity> &entity)
{
	//if ((entity->mask & MASK) == MASK)
	//{
	//}
	//if ((entity->mask & MASK) != 0) // if the entity contains any of the mask bits set (if its a light source)
	//{
	//	if ((entity->mask & IComponent::COMPONENT_DIRECTIONAL_LIGHT) == IComponent::COMPONENT_DIRECTIONAL_LIGHT)
	//	{
	//		std::shared_ptr<ComponentDirectionalLight> directionLightComp = std::dynamic_pointer_cast<ComponentDirectionalLight> (entity->FindComponent(4));
	//	}
	//	else if ((entity->mask & IComponent::COMPONENT_SPOTLIGHT) == IComponent::COMPONENT_SPOTLIGHT)
	//	{
	//		std::shared_ptr<ComponentSpotlight> spotLightComp = std::dynamic_pointer_cast<ComponentSpotlight> (entity->FindComponent(8));
	//	}
	//	else if ((entity->mask & IComponent::COMPONENT_POINTLIGHT) == IComponent::COMPONENT_POINTLIGHT)
	//	{
	//		std::shared_ptr<ComponentPointLight> pointLightComp = std::dynamic_pointer_cast<ComponentPointLight> (entity->FindComponent(16));
	//	}
	//}

	IComponent::ComponentFlags RenderableMask = (IComponent::ComponentFlags)(IComponent::COMPONENT_RIGID_BODY | IComponent::COMPONENT_RENDERABLE);

	if ((entity->mask & RenderableMask) == RenderableMask) // if the entity will be rendered
	{
		std::shared_ptr<ComponentRenderable> renderableComp = std::dynamic_pointer_cast<ComponentRenderable> (entity->FindComponent(2));
		// Assign the lighting shader to the entity
		renderableComp->lightingShader = lightingShader;
	}
}
void SystemLighting::OnTickStart(const std::shared_ptr<Entity>& entity)
{
	if ((entity->mask & IComponent::COMPONENT_SHADOW_CASTER) == IComponent::COMPONENT_SHADOW_CASTER)
	{
		std::shared_ptr<ComponentShadowCast> shadowCastComp = std::dynamic_pointer_cast<ComponentShadowCast> (entity->FindComponent(128));

		lightingShader->use();
		// Setting the lighting shader variables related to the directional light source shadows
		lightingShader->setMat4("lightSpaceMatrix", shadowCastComp->lightSpaceMatrix);
		// Binding the shadow caster 2D depth texture to the shader.
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, shadowCastComp->directionalShadowDepthMap);
	}
}
void SystemLighting::OnTickStart()
{
	// Set up the lighting shader for rendering the lights
	lightingShader->use();
	lightingShader->setVec3("viewPos", camera->Position);
	lightingShader->setFloat("time", glfwGetTime());

	float aspectRatio = (float)(SCR_WIDTH) / (float)(SCR_HEIGHT);

	glm::mat4 proj = glm::perspective(glm::radians(camera->Zoom), aspectRatio, viewDistanceNear, viewDistanceFar);

	// Set matrices in the uniform buffer object (sets the projection and view matrices in the lighting and light box shaders)
	glBindBuffer(GL_UNIFORM_BUFFER, ShaderUBO);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(proj));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->GetViewMatrix()));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Clear the viewport and buffers after shadow system operations preparing for rendering
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void SystemLighting::Tick(const std::shared_ptr<Entity> &entity)
{
	if ((entity->mask & MASK) != 0) // if the entity contains any of the mask bits set
	{
		if ((entity->mask & IComponent::COMPONENT_DIRECTIONAL_LIGHT) == IComponent::COMPONENT_DIRECTIONAL_LIGHT)
		{
			std::shared_ptr<ComponentDirectionalLight> directionLightComp = std::dynamic_pointer_cast<ComponentDirectionalLight> (entity->FindComponent(4));

			float radius = 30, angle = glfwGetTime() / 15;
			float x = radius * glm::cos(angle);
			float y = radius * glm::sin(angle);

			glm::vec3 sunPosition(x, y, 0.0f);
			glm::vec3 sunDirection = glm::normalize(glm::vec3(0.0f) - sunPosition);

			directionLightComp->position = sunPosition;
			directionLightComp->direction = sunDirection;

			lightingShader->setVec3("dirLight.direction", directionLightComp->direction);
			lightingShader->setVec3("dirLight.ambient", directionLightComp->ambient);
			lightingShader->setVec3("dirLight.diffuse", directionLightComp->diffuse);
			lightingShader->setVec3("dirLight.specular", directionLightComp->specular);

			if (drawLightSources)
				RenderLightBox(glm::vec3(sunPosition));
		}
		else if ((entity->mask & IComponent::COMPONENT_SPOTLIGHT) == IComponent::COMPONENT_SPOTLIGHT)
		{
			std::shared_ptr<ComponentSpotlight> spotLightComp = std::dynamic_pointer_cast<ComponentSpotlight> (entity->FindComponent(8));

			// center of scene
			lightingShader->setVec3("spotlight.position", spotLightComp->position);
			lightingShader->setVec3("spotlight.direction", spotLightComp->direction);

			// Emit from camera
		/*	lightingShader->setVec3("spotlight.position", camera->Position);
			lightingShader->setVec3("spotlight.direction", camera->Front);*/

			lightingShader->setVec3("spotlight.ambient", spotLightComp->ambient);
			lightingShader->setVec3("spotlight.diffuse", spotLightComp->diffuse);
			lightingShader->setVec3("spotlight.specular", spotLightComp->specular);
			lightingShader->setFloat("spotlight.constant", spotLightComp->constant);
			lightingShader->setFloat("spotlight.linear", spotLightComp->linear);
			lightingShader->setFloat("spotlight.quadratic", spotLightComp->quadratic);
			lightingShader->setFloat("spotlight.cutOff", spotLightComp->innerCutOff);
			lightingShader->setFloat("spotlight.outerCutOff", spotLightComp->outerCutOff);

			if (drawLightSources)
				RenderLightBox(spotLightComp->position);
		}
		else if ((entity->mask & IComponent::COMPONENT_POINTLIGHT) == IComponent::COMPONENT_POINTLIGHT)
		{
			std::shared_ptr<ComponentPointLight> pointLightComp = std::dynamic_pointer_cast<ComponentPointLight> (entity->FindComponent(16));

			int number = 0;
			std::string num = std::to_string(number);

			lightingShader->setVec3("pointLights[" + num + "].position", pointLightComp->position);
			lightingShader->setVec3("pointLights[" + num + "].ambient", pointLightComp->ambient);
			lightingShader->setVec3("pointLights[" + num + "].diffuse", pointLightComp->diffuse);
			lightingShader->setVec3("pointLights[" + num + "].specular", pointLightComp->specular);
			lightingShader->setFloat("pointLights[" + num + "].constant", pointLightComp->constant);
			lightingShader->setFloat("pointLights[" + num + "].linear", pointLightComp->linear);
			lightingShader->setFloat("pointLights[" + num + "].quadratic", pointLightComp->quadratic);

			if (drawLightSources)
				RenderLightBox(pointLightComp->position);
		}
	}
}


void SystemLighting::SetupShader()
{
	lightingShader->use();

	// Setting the locations for the corresponding glActiveTexture and glBindTexture calls
	lightingShader->setInt("material.diffuseMap", 0);
	lightingShader->setInt("material.specularMap", 1);
	lightingShader->setInt("normalMap", 2);
	lightingShader->setInt("shadowMap", 4); // todo: rename this to the directional shadow texture
	lightingShader->setInt("omniShadowMap", 5);

	unsigned int uniformBlockIndexLighting = glGetUniformBlockIndex(lightingShader->ID, "Matrices");
	unsigned int uniformBlockIndexLightBox = glGetUniformBlockIndex(lightBoxShader.ID, "Matrices");

	glUniformBlockBinding(lightingShader->ID, uniformBlockIndexLighting, 0);
	glUniformBlockBinding(lightBoxShader.ID, uniformBlockIndexLightBox, 0);

	// Creating the actual uniform buffer object and binding it to the binding point
	glGenBuffers(1, &ShaderUBO);

	glBindBuffer(GL_UNIFORM_BUFFER, ShaderUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Binding the UBO to the binding point 0 for all shaders
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, ShaderUBO, 0, 2 * sizeof(glm::mat4));
}
void SystemLighting::RenderLightBox(const glm::vec3 & pLightBoxPosition)
{
	// Draw the light box
	lightBoxShader.use();

	glm::mat4 model(1.0f);
	model = glm::translate(model, pLightBoxPosition);
	model = glm::scale(model, glm::vec3(0.2f));
	lightBoxShader.setMat4("model", model);

	lightBox->render();
}