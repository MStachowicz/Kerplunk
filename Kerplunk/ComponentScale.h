#ifndef COMPONENTSCALE_H
#define COMPONENTSCALE_H

#include "IComponent.h"
#include <glm/glm.hpp>

class ComponentScale : public IComponent
{
public:
	glm::vec3 scale;

	ComponentScale();
	ComponentScale(float x, float y, float z);
	ComponentScale(glm::vec3 pos);

	~ComponentScale();
};
#endif
