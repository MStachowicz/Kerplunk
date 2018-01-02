#ifndef ICOMPONENT_H
#define ICOMPONENT_H

// #include <bitset> try bitset instead of enum if enum masking isnt working

class IComponent
{
public:
	enum ComponentFlags {						// BITS					// VAL			
		COMPONENT_NONE = 0,						// 0000 0000 0000 0000		0
		COMPONENT_POSITION = 1 << 1,			// 0000 0000 0000 0010		2
		COMPONENT_ROTATION = 1 << 2,			// 0000 0000 0000 0100		4
		COMPONENT_SCALE = 1 << 3,				// 0000 0000 0000 1000		8
		COMPONENT_VELOCITY = 1 << 4,			// 0000 0000 0001 0000		16
		COMPONENT_MODEL = 1 << 5,				// 0000 0000 0010 0000		32	
		COMPONENT_GEOMETRY = 1 << 6,			// 0000 0000 0100 0000		64 
		COMPONENT_SHADER = 1 << 7,				// 0000 0000 1000 0000		128
		COMPONENT_TEXTURE = 1 << 8,				// 0000 0001 0000 0000		256
		COMPONENT_MATERIAL = 1 << 9,			// 0000 0010 0000 0000		512
		COMPONENT_LIGHT_EMITTER = 1 << 10,		// 0000 0100 0000 0000		1024
		COMPONENT_LIGHT_ATTENUATION = 1 << 11,	// 0000 1000 0000 0000		2048
		COMPONENT_LIGHT_DIRECTION = 1 << 12		// 0001 0000 0000 0000		4096

	}; ComponentFlags componentType;

	IComponent(ComponentFlags componentType);
	virtual ComponentFlags GetComponentType();
	~IComponent();
};
#endif