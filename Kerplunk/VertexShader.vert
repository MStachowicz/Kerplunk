#version 330 core
layout (location = 0) in vec3 aPos;		// the position variable has attribute position 0
layout (location = 1) in vec3 aColor;	// colour variable has attribute position 1
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor; // specify a color output to the fragment shader
out vec2 TexCoord;

uniform float horizontalOffset;

void main()
		{
			gl_Position = vec4(aPos.x + horizontalOffset, aPos.y, aPos.z, 1.0) * vec4(-1.0, -1.0,-1.0, 1.0); 
			ourColor = vec3(aColor.r + horizontalOffset, aColor.g - horizontalOffset ,aColor.b);
			TexCoord = aTexCoord;
		};