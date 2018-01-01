#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

#include "IComponent.h"
#include "Model.h"

class ComponentModel : public IComponent
{
public:
	//Model model;
	std::shared_ptr<Model> model;

	ComponentModel(std::string &path, bool gamma);
	~ComponentModel();
};
#endif