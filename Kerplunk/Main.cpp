#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <direct.h>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
// Boolean keeping track of whether wireframE mode is enabled.
bool isWireFrameModeActive = false;
// settings
const GLint SCR_WIDTH = 800, SCR_HEIGHT = 600;
// Value used for the field of view argument in the glm perspective matrix creation.
float FOV = 45.0f, cameraSpeed = 0.0f;

// Camera
// Vector pointing to the position of the camera.
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
// Vector pointing to the position the camera is looking.
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
// The inverse of the vector in which direction the camera is looking. Unit vector in direction of camera view.
glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);

// Unit vector pointing up.
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
// The cross product of the vector pointing up and the camera's direction giving the unit vector in the direction right of the camera.
glm::vec3 cameraRight = glm::normalize(glm::cross(up,cameraDirection));
// Unit vector in direction up from where the camera is viewing.
glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));

//FPS camera
// Direction vector ensuring the camera keeps looking in target direction altered by the mouse input.
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2; // Previous values of mouse position to calculate offset. Initial value centre of the screen.
bool firstMouse = true; // Prevents the camera jumping when first position is read to not be the centre of screen.
float yaw = 0.0f, pitch = 0.0f;


//Coordinate systems
// Model matrix, transforms the vertex coordinates to world coordinates.
glm::mat4 model;
// View matrix, transforms the entire scene to allow for camera movement.
glm::mat4 view;
// Projection matrix, enables openGL to create perspective using the homogeneous w component of vertices.
glm::mat4 proj = glm::perspective(glm::radians(FOV), ((float)(SCR_WIDTH / SCR_HEIGHT)), 0.1f, 100.0f);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

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

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Set openGL flag to enable depth testing.
	glEnable(GL_DEPTH_TEST);

	// Shader
	Shader ourShader("../Kerplunk/VertexShader.vert", "../Kerplunk/FragmentShader.frag");


	// Setting up mouse input 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	glfwSetCursorPosCallback(window, mouse_callback);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	glfwSetScrollCallback(window, scroll_callback);

	//float vertices[] = {
	//	// positions          // colors           // texture coords
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,		  // top right
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,		  // bottom right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,		  // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f		  // top left 
	//};

	//// Indices for triangle 1
	//unsigned int triangle1Indices[] = {  // note that we start from 0!
	//	1, 2, 3
	//};
	//// Indices for triangle 2
	//unsigned int triangle2Indices[] = {  // note that we start from 0!
	//	3, 0, 1
	//};

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

	//// Triangle 1
	//glBindVertexArray(VAO[0]); // 1. bind Vertex Array Object
	//glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // 2. copy vertices array in a buffer for OpenGL to use
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]); // 3. Copy index array in a element buffer for OpenGL to use
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle1Indices), triangle1Indices, GL_STATIC_DRAW);
	//// Position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 4. Linking vertex attribute pointers
	//glEnableVertexAttribArray(0);
	//// Texture attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	//// Triangle 2 
	//glBindVertexArray(VAO[1]); // 1. bind Vertex Array Object
	//glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); // 2. copy vertices array in a buffer for OpenGL to use
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]); // 3. Copy index array in a element buffer for OpenGL to use
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle2Indices), triangle2Indices, GL_STATIC_DRAW);
	//// Position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); 
	//glEnableVertexAttribArray(0);
	//// Texture attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	//// Unbind EBO 
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


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


	// unbind VAO and VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	// Texture
	//------------------------------------------------
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

	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	// Setting the uniform coordinate matrices in the shader.
	ourShader.setMatrix4("model", glm::value_ptr(model));
	ourShader.setMatrix4("view", glm::value_ptr(view));
	ourShader.setMatrix4("projection", glm::value_ptr(proj));



	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		// Binding textures on corresponding texture units after activating them
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		// Previous triangle render code using EBO
		/*glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);*/

		// FPS camera 
		view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, up);
		ourShader.setMatrix4("view", glm::value_ptr(view));
		// Zoom
		proj = glm::perspective(glm::radians(FOV), ((float)(SCR_WIDTH / SCR_HEIGHT)), 0.1f, 100.0f);
		ourShader.setMatrix4("projection", glm::value_ptr(proj));

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i * currentFrame;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMatrix4("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(VAO[2]);
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


// A flag set to true when the caps lock key is pressed, reset to false when the
// key is released
bool capsFlag = false;


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	cameraSpeed = 2.5f * deltaTime;


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

	// Camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPosition += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPosition -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // normalized as result vector can change based on camera front causing camera movement to affect speed
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
		//glfwSetCursorPos(window, lastX, lastY);
	}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.05;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (FOV >= 1.0f && FOV <= 45.0f)
		FOV -= yoffset;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV >= 45.0f)
		FOV = 45.0f;
}

