#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

#include "IComponent.h"
#include "Model.h"

class ComponentModel : public IComponent
{
public:
	//Model model;
	std::shared_ptr<Model> model;
	bool useModelTextures; // When set to true the model will be rendered using the textures assigned by the model file instead of material or texture component.

    // Assigns a model loaded in through the path, also sets if the entity will use the model textures and whether to gamma correct them.
	ComponentModel(std::string &path, bool gamma, bool useOriginalTextures); 
	~ComponentModel();
};
#endif