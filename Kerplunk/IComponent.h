#ifndef ICOMPONENT_H
#define ICOMPONENT_H

// #include <bitset> try bitset instead of enum if enum masking isnt working

class IComponent
{
public:
	enum ComponentFlags {			 // BITS	   // VAL			
		COMPONENT_NONE = 0,			 // 0000 0000     0
		COMPONENT_POSITION = 1 << 1, // 0000 0010	  2
		COMPONENT_ROTATION = 1 << 2, // 0000 0100	  4
		COMPONENT_SCALE = 1 << 3,	 // 0000 1000	  8
		COMPONENT_VELOCITY = 1 << 4, // 0001 0000	  16
	}; ComponentFlags componentType;
	
	IComponent(ComponentFlags componentType);
	~IComponent();
};
#endif