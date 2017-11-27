#include <iostream>
#include <filesystem>
#include <direct.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);

const GLint SCR_WIDTH = 1600, SCR_HEIGHT = 1200; // Screen dimensions.
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); // FPS camera object.
float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2; // Previous mouse position on screen. 
bool isWireFrameModeActive = false; // Boolean keeping track of whether wireframe mode is enabled.
bool firstMouse = true; // Whether the mouse callback event is being performed for the first time.

// TRANSFORMATION MATRICES FOR COORDINATE SYSTEMS
glm::mat4 model; // Model matrix, transforms the vertex coordinates to world coordinates.
glm::mat4 view;	 // View matrix, transforms the entire scene to allow for camera movement.
glm::mat4 proj; // Projection matrix, enables openGL to create perspective using the homogeneous w component of vertices.

// TIMING
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// KEY FLAGS set to true when key is pressed, reset to false when the key is released
bool capsFlag = false;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f); // Position of the light in world coordinates.

int main()
{
	char buf[100];
	_getcwd(buf, 100);
	std::cout << "Working Directory: " << buf << std::endl;

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Kerplunk", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Setting up mouse input 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Configure global OpenGL state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	// Build and compile shader
	Shader lightingShader("../Kerplunk/lighting.vert", "../Kerplunk/lighting.frag"); // Shader to calculate lighting on objects
	Shader lightBoxShader("../Kerplunk/lightBox.vert", "../Kerplunk/lightBox.frag"); // Shader to draw an always white object representing light source
	Shader textureShader("../Kerplunk/texture.vert", "../Kerplunk/texture.frag"); // Shader to draw textured objects with no lighting applied
	
	Shader screenShader("../Kerplunk/frameBuffer.vert", "../Kerplunk/frameBuffer.frag"); // Shader to draw a quad overlaying the screen used by the frame buffer object
	Shader inverseShader("../Kerplunk/frameBuffer.vert", "../Kerplunk/inversion.frag"); // Shader to inverse the colours as a post processing technique applied to the frame buffer texture
	Shader greyscaleShader("../Kerplunk/frameBuffer.vert", "../Kerplunk/greyscale.frag"); // Shader to render the scene to greyscale using the frame buffer

	float cubeVertices[] = {
		// positions          // normals            // texture coords
		// Back face
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,    0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,    1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,    1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,    1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,    0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,    0.0f, 1.0f, // top-left
		 // Front face
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     1.0f, 0.0f, // bottom-right
	 	 0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     0.0f, 0.0f, // bottom-left
		 // Left face
		-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f, // top-right
		 // Right face
		 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f, // bottom-left     
		 // Bottom face
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f, // top-right
		 // Top face
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f, // top-left
		 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f  // bottom-left  
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, 4.2f, -2.5f),
		glm::vec3(-4.8f, 10.0f, -12.3f),
		glm::vec3(6.4f, 2.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, 3.0f, -2.5f),
		glm::vec3(3.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -2.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, 3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 pointLightColours[] = {
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f)
	};

	float texRepeat = 8.0f;
	float planeVertices[] = {
		// positions            // normals           // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		// Bottom triangle
		-5.0f, -0.5f, -5.0f,    0.0f, 1.0f, 0.0f,    0.0f, texRepeat,	     // top left
		-5.0f, -0.5f,  5.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f,			 // bottom left
		 5.0f, -0.5f,  5.0f,    0.0f, 1.0f, 0.0f,    texRepeat, 0.0f,		 // bottom right
		 // Top triangle
		-5.0f, -0.5f, -5.0f,    0.0f, 1.0f, 0.0f,    0.0f, texRepeat,		 // top left
		 5.0f, -0.5f,  5.0f,    0.0f, 1.0f, 0.0f,    texRepeat, 0.0f,		 // bottom right
		 5.0f, -0.5f, -5.0f,    0.0f, 1.0f, 0.0f,    texRepeat, texRepeat    // top right
	};

	float transparentVertices[] = {
		// positions          // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,   0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,   0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,   1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,   0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,   1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,   1.0f,  0.0f
	};
	// Vegetation locations
	vector<glm::vec3> vegetation
	{
		glm::vec3(-1.5f, -1.5f, -0.48f),
		glm::vec3(1.5f, -1.5f, 0.51f),
		glm::vec3(0.0f, -1.5f, 0.7f),
		glm::vec3(-0.3f, -1.5f, -2.3f),
		glm::vec3(0.5f, -1.5f, -0.6f)
	};
	// vertex attributes for a quad that fills the entire screen in NDC.
	float quadVertices[] = { 
		// positions     // texCoords
		-1.0f,  1.0f,    0.0f, 1.0f, // top left
		-1.0f, -1.0f,    0.0f, 0.0f, // bottom left
		 1.0f, -1.0f,    1.0f, 0.0f, // bottom right

		-1.0f,  1.0f,    0.0f, 1.0f, // top left
		 1.0f, -1.0f,    1.0f, 0.0f, // bottom right
		 1.0f,  1.0f,    1.0f, 1.0f  // top right 
	};
	//for (unsigned int i = 0; i < sizeof(quadVertices) / sizeof(quadVertices[0]); i++)
	//{
	//	quadVertices[i] *= 0.5;
	//}

	// Cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Light box VAO
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	// set the vertex attributes (only position data for lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// transparent VAO
	unsigned int transparentVAO, transparentVBO;
	glGenVertexArrays(1, &transparentVAO);
	glGenBuffers(1, &transparentVBO);

	glBindVertexArray(transparentVAO);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// unbind VAO and VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Creating a frame buffer
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Creating a texture image as a colour attachment for the framebuffer 
	unsigned int texColourBuffer;
	glGenTextures(1, &texColourBuffer);
	glBindTexture(GL_TEXTURE_2D, texColourBuffer); // add missing texture active call

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attaching the texture to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColourBuffer, 0);
	

	// attaching stencil + depth buffer as a single texture attachment
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	// attaching stencil and depth buffer to the frame buffer
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureFBO, 0);

	// creating a render buffer object to attach to the frame buffer, write only, best suited to stencil and depth testing.
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	// unbinding the frame buffer object
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Load texture
	unsigned int diffuseMap = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");
	unsigned int floorTexture = loadTexture("woodFloor.png");
	unsigned int transparentTexture = loadTexture("grass.png");
	unsigned int transparentWindowTexture = loadTexture("transparentWindow.png");

	// Shader texture configurations
	lightingShader.use();
	lightingShader.setInt("material.diffuseMap", 0);
	lightingShader.setInt("material.specularMap", 1);

	textureShader.use();
	textureShader.setInt("texture1", 0);


	// Post processing
	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	inverseShader.use();
	inverseShader.setInt("screenTexture", 0);

	greyscaleShader.use();
	greyscaleShader.setInt("screenTexture", 0);

	// load models
	Model nanosuit("C:/Users/micha/Documents/Visual Studio 2017/Projects/Kerplunk/resources/objects/nanosuit/nanosuit.obj");

	//  ------------------------------------------------ RENDER LOOP ------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Per frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// render
		// FIRST PASS
		// bind to framebuffer and draw scene as we normally would to color texture 
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for default FBO to draw screen quad)
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Projection set
		proj = glm::perspective(glm::radians(camera.Zoom), ((float)(SCR_WIDTH / SCR_HEIGHT)), 0.1f, 100.0f);
		view = camera.GetViewMatrix();

		lightingShader.use();

		lightingShader.setMat4("projection", proj);
		lightingShader.setMat4("view", view);
		lightingShader.setVec3("viewPos", camera.Position);

		// Point light motion
		for (GLuint i = 0; i < 4; i++)
		{
			pointLightPositions[i].z += (0.05 * sin(glfwGetTime() * 0.5)); // light motion
			pointLightPositions[i].y += (0.015 * sin(glfwGetTime() * 2)); // light motion
		}

		// Set the uniforms for all the point lights
		for (GLuint i = 0; i < 4; i++)
		{
			string number = to_string(i);

			glUniform3f(glGetUniformLocation(lightingShader.ID, ("pointLights[" + number + "].position").c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.ID, ("pointLights[" + number + "].ambient").c_str()), pointLightColours[i].r * 0.1f, pointLightColours[i].g * 0.1f, pointLightColours[i].b * 0.1f);
			glUniform3f(glGetUniformLocation(lightingShader.ID, ("pointLights[" + number + "].diffuse").c_str()), pointLightColours[i].r, pointLightColours[i].g, pointLightColours[i].b);
			glUniform3f(glGetUniformLocation(lightingShader.ID, ("pointLights[" + number + "].specular").c_str()), 1.0f, 1.0f, 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.ID, ("pointLights[" + number + "].constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.ID, ("pointLights[" + number + "].linear").c_str()), 0.09f);
			glUniform1f(glGetUniformLocation(lightingShader.ID, ("pointLights[" + number + "].quadratic").c_str()), 0.032f);
		}



		// Set uniforms for the directional light
		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// Set uniforms for the spotlight
		lightingShader.setVec3("spotlight.position", camera.Position);
		lightingShader.setVec3("spotlight.direction", camera.Front);
		lightingShader.setVec3("spotlight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotlight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotlight.constant", 1.0f);
		lightingShader.setFloat("spotlight.linear", 0.09);
		lightingShader.setFloat("spotlight.quadratic", 0.032);
		lightingShader.setFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// Set uniforms for the material properties
		lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setFloat("material.shininess", 32.0f);


		// Draw Nanosuit
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		lightingShader.setMat4("model", model);
		nanosuit.Draw(lightingShader);


		// Binding textures on corresponding texture units after activating them
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// Draw cubes
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 10.0f * i * currentFrame;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);

			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// floor
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture); // binding floor texture to diffuse
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0); // unbind the specular map for the floor

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(4.0f));
		lightingShader.setMat4("model", model);

		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		

		// Changing colour
		//glm::vec3 lightColor;
		//lightColor.x = sin(glfwGetTime() * 2.0f);
		//lightColor.y = sin(glfwGetTime() * 0.7f);
		//lightColor.z = sin(glfwGetTime() * 1.3f);
		//glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
		//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		//lightingShader.setVec3("light.diffuse", glm::value_ptr(diffuseColor));
		//lightingShader.setVec3("light.ambient", glm::value_ptr(ambientColor));

		// Draw light cubes
		lightBoxShader.use();

		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			lightPos = pointLightPositions[i];
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));
			lightBoxShader.setMat4("model", model);
			lightBoxShader.setMat4("view", view);
			lightBoxShader.setMat4("projection", proj);

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		//for (GLuint i = 0; i < vegetation.size(); i++)
		//{
		//	model = glm::mat4();
		//	model = glm::translate(model, vegetation[i]);
		//	textureShader.setMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}


		// transparent windows
		textureShader.use();
		textureShader.setMat4("view", view);
		textureShader.setMat4("projection", proj);
		textureShader.setBool("blend", true);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, transparentWindowTexture);

		glBindVertexArray(transparentVAO);

		// sort the transparent windows before rendering
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < vegetation.size(); i++)
		{
			float distance = glm::length(camera.Position - vegetation[i]);
			sorted[distance] = vegetation[i];
		}
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4();
			model = glm::translate(model, it->second);
			textureShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// SECOND PASS
		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		greyscaleShader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, texColourBuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	// Close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	// Toggle wireframe mode
	if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
	{
		if (!capsFlag)
		{
			if (isWireFrameModeActive)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				isWireFrameModeActive = false;
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				isWireFrameModeActive = true;
			}
		}

		// Set flag to indicate the key is being pressed.
		capsFlag = true;
	}
	// Toggle wireframe mode
	if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_RELEASE)
	{
		capsFlag = false;
	}

	// CAMERA MOVEMENT
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	int width1, height1, nrChannels1;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}