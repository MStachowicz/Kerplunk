#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include "SystemManager.h"
#include "SystemPhysics.h"
#include "SystemRender.h"
#include "EntityManager.h"
#include "Entity.h"
#include "ComponentPosition.h"
#include "ComponentRotation.h"
#include "ComponentScale.h"
#include "ComponentVelocity.h"
#include "ComponentGeometry.h"
#include "ComponentTexture.h"
#include "ComponentMaterial.h"
#include "ComponentShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTextureMain(char const * path, bool gammaCorrection);
unsigned int loadCubemap(vector<std::string> faces);
void setupLighting(shared_ptr<Shader> shader, glm::vec3 pointLightPositions[], glm::vec3 pointLightColours[], glm::vec3 pointLightSpecular[]);
void renderObjects(const Shader &shader, glm::vec3 cubePositions[], unsigned int cubeVAO, unsigned int floorTexture, bool bindTextures);
void createEntities(EntityManager &entityManager);

const GLint SCR_WIDTH = 1600, SCR_HEIGHT = 1200; // Screen dimensions.
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f)); // FPS camera object.
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
bool testFlag = false;
bool isNormalMapActive = true; // Switches the lighting to use the blinn-phong lighting model.

unsigned int brickwallTexture;
unsigned int brickwallNormalMap;
unsigned int containerDiffuseMap;
unsigned int containerSpecularMap;

Model planet;
Model nanosuit;
Model rock;

Model grass;
Model Desk;

// Component managers
SystemManager systemManager;
EntityManager entityManager;
SystemPhysics systemPhysics;
SystemRender systemRender;

std::shared_ptr<Shader> lightingShader;


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4); // increase the number of samples in buffers

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
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	lightingShader = std::make_shared<Shader>("../Kerplunk/lighting.vert", "../Kerplunk/lighting.frag", "../Kerplunk/explode.geom"); // Shader to calculate lighting on objects

	createEntities(entityManager);
	systemManager.AddSystem(systemPhysics);
	systemManager.AddSystem(systemRender);

	// Build and compile shader
	Shader lightBoxShader("../Kerplunk/lightBox.vert", "../Kerplunk/lightBox.frag", nullptr); // Shader to draw an always white object representing light source
	Shader textureShader("../Kerplunk/texture.vert", "../Kerplunk/texture.frag", nullptr); // Shader to draw textured objects with no lighting applied

	Shader screenShader("../Kerplunk/frameBuffer.vert", "../Kerplunk/frameBuffer.frag", nullptr); // Shader to draw a quad overlaying the screen used by the frame buffer object
	Shader inverseShader("../Kerplunk/frameBuffer.vert", "../Kerplunk/inversion.frag", nullptr); // Shader to inverse the colours as a post processing technique applied to the frame buffer texture
	Shader greyscaleShader("../Kerplunk/frameBuffer.vert", "../Kerplunk/greyscale.frag", nullptr); // Shader to render the scene to greyscale using the frame buffer
	Shader kernelShader("../Kerplunk/frameBuffer.vert", "../Kerplunk/kernel.frag", nullptr); // Shader to render the scene using a kernel matrix to apply post processing

	Shader skyboxShader("../Kerplunk/cubemap.vert", "../Kerplunk/cubemap.frag", nullptr); // Shader to render a cubemap reusing the position as the texture coordinates
	Shader reflectionShader("../Kerplunk/reflection.vert", "../Kerplunk/reflection.frag", nullptr); // Shader to render an object with environment reflection using cubemap texture
	Shader refractionShader("../Kerplunk/reflection.vert", "../Kerplunk/refraction.frag", nullptr); // Shader to render an object with environment refraction using cubemap texture

	Shader normalVisualizeShader("../Kerplunk/normalVisualize.vert", "../Kerplunk/normalVisualize.frag", "../Kerplunk/normalVisualize.geom"); // Shader to generate lines eminating fromt the vertices in the direction of their normals
	//Shader instancedLightingShader("../Kerplunk/lightingInstanced.vert", "../Kerplunk/lighting.frag", "../Kerplunk/explode.geom");

	Shader materialShader("../Kerplunk/materialLighting.vert", "../Kerplunk/materialLighting.frag", nullptr); // Shader to draw objects with material properties and texture applied.
	Shader simpleDepthShader("../Kerplunk/depthShader.vert", "../Kerplunk/depthShader.frag", nullptr); // Shader used for shadow mapping, used to write to the depth buffer performing no lighting. 
	Shader debugDepthQuad("../Kerplunk/debugQuad.vert", "../Kerplunk/debugQuad.frag", nullptr);
	Shader omniDepthShader("../Kerplunk/omniDepthShader.vert", "../Kerplunk/omniDepthShader.frag", "../Kerplunk/omniDepthShader.geom"); // shader used to fill the depth component of the framebuffer cubemap for omnidirectional shadow mapping

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
		glm::vec3(0.0f,  2.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, 4.2f, -2.5f),
		glm::vec3(-4.8f, 10.0f, -12.3f),
		glm::vec3(6.4f, 2.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, 3.0f, -2.5f),
		glm::vec3(3.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  2.2f, -4.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.0f,  3.0f,  -19.0f),
		glm::vec3(-6.0f,   -1.0f,  -6.0f),
		glm::vec3(-2.0f,   -1.0f,  -6.0f),
		glm::vec3(2.0f,   -1.0f,  -6.0f)
	};

	glm::vec3 pointLightColours[] = {
		glm::vec3(1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f)
	};

	glm::vec3 pointLightSpecular[] = {
		glm::vec3(0.9f),
		glm::vec3(0.7f),
		glm::vec3(0.5f),
		glm::vec3(0.3f)
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


	// Skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Reflective cube VAO
	unsigned int mirrorCubeVAO, mirrorCubeVBO;
	glGenVertexArrays(1, &mirrorCubeVAO);
	glGenBuffers(1, &mirrorCubeVBO);

	glBindVertexArray(mirrorCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mirrorCubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


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
	glBindTexture(GL_TEXTURE_2D, texColourBuffer);

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

	// creating a render buffer object to attach to the framebuffer, write only, best suited to stencil and depth testing.
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	// unbinding the framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// DIRECTIONAL SHADOW MAPPING
	// Generating a framebuffer to store the shadow depth map
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const unsigned int SHADOW_WIDTH = 1024 * 2, SHADOW_HEIGHT = 1024 * 2;

	// Creating a 2D texture to be used as the frame buffers depth buffer
	unsigned int directionalShadowDepthMap;
	glGenTextures(1, &directionalShadowDepthMap);

	glBindTexture(GL_TEXTURE_2D, directionalShadowDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // setting border to prevent shadow in samples outside the light perspective
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Attaching the depth texture to the framebuffers depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, directionalShadowDepthMap, 0);
	// No need to attach a colour buffer so setting draw and read buffer to GL_NONE to signify this.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// OMNIDIRECTIONAL SHADOW MAPPING
	const unsigned int OMNI_SHADOW_WIDTH = 1024, OMNI_SHADOW_HEIGHT = 1024;



	unsigned int depthMapOmniFBO;
	glGenFramebuffers(1, &depthMapOmniFBO);

	// Creating a cubemap
	unsigned int omniDirectionalDepthCubemap;
	glGenTextures(1, &omniDirectionalDepthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, omniDirectionalDepthCubemap);

	// generating each of the cubemap faces as 2D depth value texture images
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, OMNI_SHADOW_WIDTH, OMNI_SHADOW_HEIGHT,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// directly attaching the cubemap to the framebuffer depth attachment
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapOmniFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, omniDirectionalDepthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Load models
	//Desk = Model("C:/Users/micha/Documents/Visual Studio 2017/Projects/Kerplunk/resources/objects/Desk/deskWorn_OBJ.obj", true);
	//grass = Model("C:/Users/micha/Documents/Visual Studio 2017/Projects/Kerplunk/resources/objects/Grass pack/Grass_02.obj", true);
	//planet = Model("C:/Users/micha/Documents/Visual Studio 2017/Projects/Kerplunk/resources/objects/planet/planet.obj", true);
	//nanosuit = Model("C:/Users/micha/Documents/Visual Studio 2017/Projects/Kerplunk/resources/objects/nanosuit/nanosuit.obj", true);
	//rock = Model("C:/Users/micha/Documents/Visual Studio 2017/Projects/Kerplunk/resources/objects/rock/rock.obj", true);


	// Load texture
	containerDiffuseMap = loadTextureMain("container2.png", true);
	containerSpecularMap = loadTextureMain("container2_specular.png", false);
	unsigned int floorTexture = loadTextureMain("woodFloor.png", true);
	unsigned int transparentTexture = loadTextureMain("grass.png", true);
	unsigned int transparentWindowTexture = loadTextureMain("transparentWindow.png", true);
	brickwallTexture = loadTextureMain("brickWall.jpg", true);
	brickwallNormalMap = loadTextureMain("brickwall_normal.jpg", false);

	// Loading cube map texture
	vector<std::string> faces =
	{
		"cubemaps/hw_spires/spires_rt.tga",
		"cubemaps/hw_spires/spires_lf.tga",
		"cubemaps/hw_spires/spires_up.tga",
		"cubemaps/hw_spires/spires_dn.tga",
		"cubemaps/hw_spires/spires_bk.tga",
		"cubemaps/hw_spires/spires_ft.tga",
	};
	vector<std::string> faces2 =
	{
		"cubemaps/skybox/right.jpg",
		"cubemaps/skybox/left.jpg",
		"cubemaps/skybox/top.jpg",
		"cubemaps/skybox/bottom.jpg",
		"cubemaps/skybox/back.jpg",
		"cubemaps/skybox/front.jpg",
	};
	unsigned int cubemapTexture = loadCubemap(faces2);

	// Shader texture configurations
	lightingShader->use();
	lightingShader->setInt("material.diffuseMap", 0);
	lightingShader->setInt("material.specularMap", 1);
	lightingShader->setInt("normalMap", 2);
	lightingShader->setInt("shadowMap", 4);
	lightingShader->setInt("omniShadowMap", 5);

	materialShader.use();
	materialShader.setInt("material.diffuseMap", 0);

	textureShader.use();
	textureShader.setInt("texture1", 0);

	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);

	// Post processing
	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	inverseShader.use();
	inverseShader.setInt("screenTexture", 0);

	greyscaleShader.use();
	greyscaleShader.setInt("screenTexture", 0);

	kernelShader.use();
	kernelShader.setInt("screenTexture", 0);

	reflectionShader.use();
	reflectionShader.setInt("skybox", 0);

	refractionShader.use();
	refractionShader.setInt("skybox", 0);

	// Uniform block index
	// Set the binding point for the uniform block in the shaders
	unsigned int uniformBlockIndexLighting = glGetUniformBlockIndex(lightingShader->ID, "Matrices");
	unsigned int uniformBlockIndexLightBox = glGetUniformBlockIndex(lightBoxShader.ID, "Matrices");
	unsigned int uniformBlockIndexReflection = glGetUniformBlockIndex(reflectionShader.ID, "Matrices");
	unsigned int uniformBlockIndexRefraction = glGetUniformBlockIndex(refractionShader.ID, "Matrices");
	unsigned int uniformBlockIndexSkyBox = glGetUniformBlockIndex(skyboxShader.ID, "Matrices");
	unsigned int uniformBlockIndexTextureShader = glGetUniformBlockIndex(textureShader.ID, "Matrices");
	unsigned int uniformBlockIndexNormalShader = glGetUniformBlockIndex(normalVisualizeShader.ID, "Matrices");
	//unsigned int uniformBlockIndexInstanceShader = glGetUniformBlockIndex(instancedLightingShader.ID, "Matrices");
	unsigned int uniformBlockIndexMaterialShader = glGetUniformBlockIndex(materialShader.ID, "Matrices");

	glUniformBlockBinding(lightingShader->ID, uniformBlockIndexLighting, 0);
	glUniformBlockBinding(lightBoxShader.ID, uniformBlockIndexLightBox, 0);
	glUniformBlockBinding(reflectionShader.ID, uniformBlockIndexReflection, 0);
	glUniformBlockBinding(refractionShader.ID, uniformBlockIndexRefraction, 0);
	glUniformBlockBinding(skyboxShader.ID, uniformBlockIndexSkyBox, 0);
	glUniformBlockBinding(textureShader.ID, uniformBlockIndexTextureShader, 0);
	glUniformBlockBinding(normalVisualizeShader.ID, uniformBlockIndexNormalShader, 0);
	//glUniformBlockBinding(instancedLightingShader.ID, uniformBlockIndexInstanceShader, 0);
	glUniformBlockBinding(materialShader.ID, uniformBlockIndexMaterialShader, 0);

	// Creating the actual uniform buffer object and binding it to the binding point
	unsigned int UBOmatrices;
	glGenBuffers(1, &UBOmatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, UBOmatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// Binding the UBO to the binding point 0 for all shaders
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOmatrices, 0, 2 * sizeof(glm::mat4));


	// Generating an asteroid field
	unsigned int amount = 100000;
	float radius = 50;
	float offset = 5.5f;

	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];

	srand(glfwGetTime()); // initialize random seed	

	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;

		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = 40 + displacement * 0.4f; // keep height of field smaller compared to width of x and z

		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;

		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	// vertex Buffer Object
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < rock.meshes.size(); i++)
	{
		unsigned int VAO = rock.meshes[i].VAO;
		glBindVertexArray(VAO);
		// vertex Attributes
		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}


	//  ------------------------------------------------ RENDER LOOP ------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Per frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);;

		//// FIRST PASS
		//// bind to framebuffer and draw scene as we normally would to color texture 
		//glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		//glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for default FBO to draw screen quad)		


		// 1. render depth of scene to depth map for directional shadow mapping
		// --------------------------------------------------------------
		glm::mat4 lightProjection, lightView, lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 50.0f, projSize = 15.0f;

		lightProjection = glm::ortho(-projSize, projSize, -projSize, projSize, near_plane, far_plane);
		lightView = glm::lookAt(
			glm::vec3(-2.0f, 30.0f, -1.0f), // light position
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		// 1.1 render scene to depth map
		// --------------------------------------------------------------
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT); // reverse cull order to reduce peter panning on shadows
		renderObjects(simpleDepthShader, cubePositions, cubeVAO, floorTexture, false);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. render depth of scene to cubemap for omnidirectional shadow mapping
		// --------------------------------------------------------------
		float aspect = (float)OMNI_SHADOW_WIDTH / (float)OMNI_SHADOW_HEIGHT;
		float omniProjectionNear = 1.0f, omniProjectionFar = 25.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, omniProjectionNear, omniProjectionFar);

		int i = 0;
		// creating the light space transformation matrices for the 6 faces of cubemap
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pointLightPositions[i], pointLightPositions[i] + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pointLightPositions[i], pointLightPositions[i] + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pointLightPositions[i], pointLightPositions[i] + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pointLightPositions[i], pointLightPositions[i] + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pointLightPositions[i], pointLightPositions[i] + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(pointLightPositions[i], pointLightPositions[i] + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		// 2.1 render scene to cubemap
		// --------------------------------------------------------------
		glViewport(0, 0, OMNI_SHADOW_WIDTH, OMNI_SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapOmniFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		omniDepthShader.use();
		for (unsigned int i = 0; i < 6; ++i)
			omniDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);

		omniDepthShader.setFloat("far_plane", omniProjectionFar);
		omniDepthShader.setVec3("lightPos", pointLightPositions[0]);

		renderObjects(omniDepthShader, cubePositions, cubeVAO, floorTexture, false);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//}


		// 3. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		// reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Projection + view set
		proj = glm::perspective(glm::radians(camera.Zoom), ((float)(SCR_WIDTH / SCR_HEIGHT)), 0.1f, 100.0f);
		view = camera.GetViewMatrix();

		// Set matrices in the uniform buffer object (sets the projection and view matrices)
		glBindBuffer(GL_UNIFORM_BUFFER, UBOmatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(proj));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, UBOmatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		lightingShader->use();
		// Set up all the lighting in the scene
		setupLighting(lightingShader, pointLightPositions, pointLightColours, pointLightSpecular);
		// add time component to geometry shader in the form of a uniform
		lightingShader->setFloat("time", glfwGetTime());
		lightingShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		lightingShader->setFloat("omniFarPlane", omniProjectionFar);
		// Binding textures on corresponding texture units after activating them
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, directionalShadowDepthMap);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, omniDirectionalDepthCubemap);

		// renderObjects(lightingShader, cubePositions, cubeVAO, floorTexture, true);

		// component testing
		systemManager.ActionSystems(entityManager);

		// render Depth map of directional shadow to quad for visual debugging
		// ---------------------------------------------
		//debugDepthQuad.use();
		//debugDepthQuad.setFloat("near_plane", near_plane);
		//debugDepthQuad.setFloat("far_plane", far_plane);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, directionalShadowDepthMap); // directional
		//renderQuad();

		// Draw light cubes
		lightBoxShader.use();

		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lightBoxShader.setMat4("model", model);

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Draw reflective cube
		reflectionShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.0f, 0.5f, 0.0f));
		reflectionShader.setMat4("model", model);
		reflectionShader.setVec3("cameraPos", camera.Position);

		glBindVertexArray(mirrorCubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// draw refraction cube	
		refractionShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.0f, 0.5f, 0.0f));
		refractionShader.setMat4("model", model);
		refractionShader.setVec3("cameraPos", camera.Position);

		glBindVertexArray(mirrorCubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		// Draw cube map
		glDepthFunc(GL_LEQUAL); // set depth function so depth test passes when value is equal to 1 as is set in the cubemap shader

		skyboxShader.use();

		glm::mat4 cubeView = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translations from the view matrix
		glBindBuffer(GL_UNIFORM_BUFFER, UBOmatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cubeView));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);


		//for (GLuint i = 0; i < vegetation.size(); i++)
		//{
		//	model = glm::mat4();
		//	model = glm::translate(model, vegetation[i]);
		//	textureShader.setMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}


		// transparent windows
		//textureShader.use();

		//glBindBuffer(GL_UNIFORM_BUFFER, UBOmatrices);
		//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		//glBindBuffer(GL_UNIFORM_BUFFER, 0);
		//textureShader.setBool("blend", true);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, transparentWindowTexture);

		//glBindVertexArray(transparentVAO);

		//// sort the transparent windows before rendering
		//std::map<float, glm::vec3> sorted;
		//for (unsigned int i = 0; i < vegetation.size(); i++)
		//{
		//	float distance = glm::length(camera.Position - vegetation[i]);
		//	sorted[distance] = vegetation[i];
		//}
		//for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		//{
		//	model = glm::mat4();
		//	model = glm::translate(model, it->second);
		//	textureShader.setMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}



		//// SECOND PASS
		//// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		//// clear all relevant buffers
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		//glClear(GL_COLOR_BUFFER_BIT);

		//screenShader.use();
		//glBindVertexArray(quadVAO);
		//glBindTexture(GL_TEXTURE_2D, texColourBuffer);	// use the color attachment texture as the texture of the quad plane
		//glDrawArrays(GL_TRIANGLES, 0, 6);

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

// code testing the creation of en entity with components added 
void createEntities(EntityManager &entityManager)
{
	for (int i = 0; i < 1; i++)
	{
		Entity entity1("nanosuit");
		entity1.AddComponent(ComponentPosition(glm::vec3(0.0f + i, 1.0f, -20.0f)));
		entity1.AddComponent(ComponentRotation(glm::vec3(1.0f)));
		entity1.AddComponent(ComponentScale(glm::vec3(0.2f)));
		entity1.AddComponent(ComponentVelocity(glm::vec3(0.0f, 0.0f, 0.0f)));
		std::string filepath = "C:/Users/Michal/Source/Repos/Kerplunk/Kerplunk/Resources/Objects/nanosuit/nanosuit.obj";
		entity1.AddComponent(ComponentModel(filepath, true, true));
		entity1.AddComponent(ComponentShader(lightingShader));
		entity1.AddComponent(ComponentMaterial(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0), 64.0f));
		entityManager.AddEntity(entity1);
	}

	Entity entity2("cube");
	entity2.AddComponent(ComponentPosition(glm::vec3(0.0f, 1.0f, -20.0f)));
	entity2.AddComponent(ComponentRotation(glm::vec3(1.0f)));
	entity2.AddComponent(ComponentScale(glm::vec3(1.0f)));
	entity2.AddComponent(ComponentVelocity(glm::vec3(0.0f, 0.0f, 0.0f)));
	entity2.AddComponent(ComponentGeometry("C:/Users/Michal/Source/Repos/Kerplunk/Kerplunk/Cube.txt"));
	entity2.AddComponent(ComponentShader(lightingShader));
	ComponentTexture texture("container2.png", true);
	texture.AddSpecularTexture("container2_specular.png");
	entity2.AddComponent(texture);
	entityManager.AddEntity(entity2);

	Entity entity3("sphere");
	entity3.AddComponent(ComponentPosition(glm::vec3(2.0f, 1.0f, -20.0f)));
	entity3.AddComponent(ComponentRotation(glm::vec3(1.0f)));
	entity3.AddComponent(ComponentScale(glm::vec3(1.0f)));
	entity3.AddComponent(ComponentVelocity(glm::vec3(0.0f, 0.0f, 0.0f)));
	std::string filepath = "C:/Users/Michal/Source/Repos/Kerplunk/Kerplunk/Resources/Model primitives/Ico Sphere/4 subdivisions/Ico Sphere.obj";
	entity3.AddComponent(ComponentModel(filepath, true, false));
	entity3.AddComponent(ComponentShader(lightingShader));
	entity3.AddComponent(ComponentMaterial(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0), 64.0f));
	entityManager.AddEntity(entity3);
}

void renderObjects(const Shader &shader, glm::vec3 cubePositions[], unsigned int cubeVAO, unsigned int floorTexture, bool bindTextures)
{
	if (bindTextures)
	{
		shader.setBool("isNormalMap", false);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, containerDiffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, containerSpecularMap);
	}

	// floating cubes
	for (unsigned int i = 0; i < 10; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 10.0f * i * glfwGetTime();
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shader.setMat4("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// wooden floor
	if (bindTextures)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture); // binding floor texture to diffuse
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, NULL); // binding blank specular texture map
	}

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f));

	//renderQuad(shader, model);

	// Brick wall
	if (bindTextures)
	{
		shader.setBool("isNormalMap", isNormalMapActive); // Enables normal mapping in the shader if toggled on

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brickwallTexture); // binding floor texture to diffuse
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, NULL); // binding blank specular texture map
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, brickwallNormalMap); // binding normal map texture
	}


	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 3.0f, -20.0f));
	model = glm::scale(model, glm::vec3(20.0f));

	//renderQuad(shader, model);

	if (bindTextures)
		shader.setBool("isNormalMap", false);

	// Draw Nanosuit
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, -18.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4("model", model);
	nanosuit.Draw(shader);

	// Draw iron man
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -18.0f));
	//model = glm::scale(model, glm::vec3(0.2f));
	//shader.setMat4("model", model);
	//ironMan.Draw(shader);

	// Draw cyborg
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.0f, 0.0f, -18.0f));
	model = glm::scale(model, glm::vec3(0.08f));
	shader.setMat4("model", model);
	Desk.Draw(shader);

	// Draw grass
	glDisable(GL_CULL_FACE);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, -18.0f));
	model = glm::scale(model, glm::vec3(1));
	shader.setMat4("model", model);
	grass.Draw(shader);

	glEnable(GL_CULL_FACE);


	//// Redraw nanosuit drawing the normals away from its vertices
	//normalVisualizeShader.use();
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(5.0f, -2.0f, -10.0f));
	//model = glm::scale(model, glm::vec3(0.2f));
	//normalVisualizeShader.setMat4("model", model);
	//nanosuit.Draw(normalVisualizeShader);


	//// draw Planet
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//lightingShader.setMat4("model", model);
	//planet.Draw(lightingShader);

	//// draw meteorites
	//instancedLightingShader.use();
	//for (unsigned int i = 0; i < rock.meshes.size(); i++)
	//{
	//	glBindVertexArray(rock.meshes[i].VAO);
	//	glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
	//}
}

void setupLighting(shared_ptr<Shader> shader, glm::vec3 pointLightPositions[], glm::vec3 pointLightColours[], glm::vec3 pointLightSpecular[])
{
	// Set up all the lighting in the scene
	shader->use();
	shader->setVec3("viewPos", camera.Position);

	// Point light motion
	for (GLuint i = 0; i < 1; i++)
	{
		//pointLightPositions[i].x += (0.05 * sin(glfwGetTime() * 0.5)); // light motion
		//pointLightPositions[i].z += (0.05 * sin(glfwGetTime() * 0.5)); // light motion
		//pointLightPositions[i].y += (0.015 * sin((glfwGetTime() * 2) + 1)); // light motion
	}

	// Set the uniforms for all the point lights
	for (GLuint i = 0; i < 4; i++)
	{
		string number = to_string(i);

		glUniform3f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].position").c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
		glUniform3f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].ambient").c_str()), pointLightColours[i].r, pointLightColours[i].g, pointLightColours[i].b);
		glUniform3f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].diffuse").c_str()), pointLightColours[i].r, pointLightColours[i].g, pointLightColours[i].b);
		glUniform3f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].specular").c_str()), pointLightSpecular[i].r, pointLightSpecular[i].g, pointLightSpecular[i].b);
		glUniform1f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].constant").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].linear").c_str()), 0.9f);
		glUniform1f(glGetUniformLocation(shader->ID, ("pointLights[" + number + "].quadratic").c_str()), 0.32f);
	}

	// Set uniforms for the directional light
	shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader->setVec3("dirLight.ambient", 0.01f, 0.01f, 0.01f);
	shader->setVec3("dirLight.diffuse", 0.01f, 0.01f, 0.01f);
	shader->setVec3("dirLight.specular", 0.1f, 0.1f, 0.1f);

	// Set uniforms for the spotlight
	shader->setVec3("spotlight.position", -15.793399f, -0.271867f, 15.654298f);
	//shader->setVec3("spotlight.position", camera.Position);
	shader->setVec3("spotlight.direction", 0.905481f, -0.424199f, -0.012639f);
	//shader->setVec3("spotlight.direction", camera.Front);
	shader->setVec3("spotlight.diffuse", 1.0f, 1.0f, 1.0f);
	shader->setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("spotlight.constant", 1.0f);
	shader->setFloat("spotlight.linear", 0.09);
	shader->setFloat("spotlight.quadratic", 0.032);
	shader->setFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
	shader->setFloat("spotlight.outerCutOff", glm::cos(glm::radians(15.0f)));

	// Set uniforms for the material properties
	shader->setVec3("material.ambient", 0.0f, 0.0f, 0.0f);
	shader->setVec3("material.diffuse", 0.5f, 0.5f, 0.5f);
	shader->setVec3("material.specular", 0.2f, 0.2f, 0.2f);
	shader->setFloat("material.shininess", 32.0f);
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

	// Toggle blinn shading
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		if (!testFlag)
		{
			if (isNormalMapActive)
				isNormalMapActive = false;
			else
				isNormalMapActive = true;
		}

		// Set flag to indicate the key is being pressed.
		testFlag = true;
	}
	// Toggle blinn shading
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE)
	{
		testFlag = false;
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
unsigned int loadTextureMain(char const * path, bool gammaCorrection)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		if (nrComponents == 1)
		{
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

// Loads a cube map from 6 different textures held in a vector of faces. Faces must be in order 
// right, left, top, bottom, back, front.
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}