#ifndef ICOMPONENT_H
#define ICOMPONENT_H

class IComponent
{
public:

	enum ComponentFlags {			 // BITS	   // VAL	
		COMPONENT_NONE = 1,			 // 0000 0001     1
		COMPONENT_POSITION = 1 << 1, // 0000 0010	  2
		COMPONENT_ROTATION = 1 << 2, // 0000 0100	  4
		COMPONENT_SCALE = 1 << 3,	 // 0000 1000	  8
	};

	// Forces components to define their type for building masks.
	virtual const ComponentFlags ComponentType() = 0;

private:
	ComponentFlags componentType = COMPONENT_NONE;

	~IComponent();
};
#endif