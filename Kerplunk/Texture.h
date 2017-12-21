#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include "stb_image.h"
#include <iostream>
#include <string>

class Texture
{
public: 
	unsigned int textureID;

	Texture();
	~Texture();
	unsigned int LoadTexture(std::string &path, bool gammaCorrection);
};

#endif 