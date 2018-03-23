#ifndef COMPONENTRENDERABLE_H
#define COMPONENTRENDERABLE_H

#include "IComponent.h"
#include "Shader.h"
#include <memory>

class ComponentRenderable : public IComponent
{
public:
	ComponentRenderable();

	// Shader assigned by the lighting system to perform rendering of objects and setting light entity values
	std::weak_ptr<Shader> lightingShader;
	// Whether this renderable entity emits shadows when hit by a light source.
	bool emitShadows = true;

	~ComponentRenderable();
};
#endif