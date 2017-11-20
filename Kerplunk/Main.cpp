#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <direct.h>
#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const GLint SCR_WIDTH = 800, SCR_HEIGHT = 600; // Screen dimensions.
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

	// Build and compile shader
	Shader ourShader("../Kerplunk/VertexShader.vert", "../Kerplunk/FragmentShader.frag"); // Shader featuring texture mapping with two textures
	Shader lightingShader("../Kerplunk/VertexShader - lighting.vert", "../Kerplunk/FragmentShader - lighting.frag"); // Shader to calculate lighting on objects
	Shader lightBoxShader("../Kerplunk/VertexShader - lightBox.vert", "../Kerplunk/FragmentShader - lightBox.frag"); // Shader to draw an always white object representing light source

	float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// Setting up VAOs and buffers
	unsigned int VAO[3], VBO[3], EBO[2];
	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(2, EBO);

	// Cube object
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Setting up data for light box
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	// set the vertex attributes (only position data for lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unbind VAO and VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ------------------------------------------------ TEXTURE ------------------------------------------------
	// Generate texture objects
	unsigned int texture[2];
	glGenTextures(2, texture);

	// Texture 1
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Setting the texture wrapping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// Setting the texture filtering option to linear mipmap for downscaled and linear for upscaled textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Loading image using stb_image
	int width1, height1, nrChannels1;
	unsigned char *data1 = stbi_load("WoodContainer.jpg", &width1, &height1, &nrChannels1, 0);

	if (data1)
	{
		// Attach texture image/data to texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 1" << std::endl;
	}
	// Free image memory
	stbi_image_free(data1);


	// Texture 2
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	// Setting the texture wrapping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// Setting the texture filtering option to linear mipmap for downscaled and linear for upscaled textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Loading image using stb_image
	int width2, height2, nrChannels2;
	unsigned char *data2 = stbi_load("brickWall.jpg", &width2, &height2, &nrChannels2, 0);

	if (data2)
	{
		// Attach texture image/data to texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 2" << std::endl;
	}
	// Free image memory
	stbi_image_free(data2);

	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

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
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//ourShader.use();
		//// Binding textures on corresponding texture units after activating them
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture[0]);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture[1]);

		// Projection set
		proj = glm::perspective(glm::radians(camera.Zoom), ((float)(SCR_WIDTH / SCR_HEIGHT)), 0.1f, 100.0f);
		view = camera.GetViewMatrix();

		//ourShader.setMatrix4("view", glm::value_ptr(view));
		//ourShader.setMatrix4("projection", glm::value_ptr(proj));

		lightingShader.use();

		lightingShader.setMatrix4("projection", glm::value_ptr(proj));
		lightingShader.setMatrix4("view", glm::value_ptr(view));

		glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("objectColor", glm::value_ptr(objectColor));
		lightingShader.setVec3("lightColor", glm::value_ptr(lightColor));

		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i * currentFrame;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMatrix4("model", glm::value_ptr(model));

			glBindVertexArray(VAO[2]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// LIGHT CUBE + shader
		lightBoxShader.use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightBoxShader.setMatrix4("model", glm::value_ptr(model));
		lightBoxShader.setMatrix4("projection", glm::value_ptr(proj));
		lightBoxShader.setMatrix4("view", glm::value_ptr(view));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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

