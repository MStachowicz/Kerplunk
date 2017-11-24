#include <iostream>
#include <filesystem>
#include <direct.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);

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
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 pointLightColours[] = {
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f)
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Setting up data for light box
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	// set the vertex attributes (only position data for lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unbind VAO and VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Load texture
	unsigned int diffuseMap = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");
	lightingShader.use();
	lightingShader.setInt("material.diffuseMap", 0);
	lightingShader.setInt("material.specularMap", 1);

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		// Binding textures on corresponding texture units after activating them
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// Projection set
		proj = glm::perspective(glm::radians(camera.Zoom), ((float)(SCR_WIDTH / SCR_HEIGHT)), 0.1f, 100.0f);
		view = camera.GetViewMatrix();

		lightingShader.use();

		lightingShader.setMat4("projection", proj);
		lightingShader.setMat4("view", view);
		lightingShader.setVec3("viewPos", camera.Position);

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


		// Changing colour
		//glm::vec3 lightColor;
		//lightColor.x = sin(glfwGetTime() * 2.0f);
		//lightColor.y = sin(glfwGetTime() * 0.7f);
		//lightColor.z = sin(glfwGetTime() * 1.3f);
		//glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
		//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		//lightingShader.setVec3("light.diffuse", glm::value_ptr(diffuseColor));
		//lightingShader.setVec3("light.ambient", glm::value_ptr(ambientColor));


		// Draw cubes
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 10.0f * i * currentFrame;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);

			glBindVertexArray(VAO[2]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		// Draw light cubes
		lightBoxShader.use();

		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			lightPos = pointLightPositions[i];
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));
			lightBoxShader.setMat4("model", model);

			lightBoxShader.setMat4("projection", proj);
			lightBoxShader.setMat4("view", view);

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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


