#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include "Shader.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int generateObject();
unsigned int createVao(unsigned int vbo, float* vertices, unsigned int size);
unsigned int createEbo(unsigned int* indices, unsigned int size);
void drawSomething(unsigned int vao, unsigned int ebo, int indexCount);

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) {
		printf("Something went wrong!\n");
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glViewport(0, 0, 800, 600);

	unsigned int texture = SOIL_load_OGL_texture
	(
		"image.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS
	);

	if (texture <= 0)
		std::cout << "Texture loading failed: '" << SOIL_last_result() << "'" << std::endl;
	else
		glActiveTexture(GL_TEXTURE0);

	Shader shader("VertexShader.glsl", "FragmentShader.glsl");

	float vertices[] = {
		// Face 1 (front)
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
		// Face 2 (back)
		// positions          // colors           // texture coords
		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
		// Face 3 (left side)
		// positions          // colors           // texture coords
		-0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // top right
		-0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,    // top left 
		// Face 4 (right side)
		// positions          // colors           // texture coords
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		 0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
		 0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
		// Face 5 (bottom)
		// positions          // colors           // texture coords
		-0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 1.0f,    // top left 
		// Face 6 (top)
		// positions          // colors           // texture coords
		-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // top right
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3,   // second triangle

		4, 5, 7,   // first triangle
		5, 6, 7,    // second triangle

		8, 9, 11,   // first triangle
		9, 10, 11,    // second triangle

		12, 13, 15,   // first triangle
		13, 14, 15,    // second triangle

		16, 17, 19,   // first triangle
		17, 18, 19,    // second triangle

		20, 21, 23,   // first triangle
		21, 22, 23,    // second triangle
	};

	unsigned int vertexCount = sizeof(vertices);
	unsigned int indexCount = sizeof(indices);
	unsigned int vbo = generateObject(); // Vertex Buffer Object
	unsigned int vao = createVao(vbo, vertices, vertexCount); // Vertex Array Object
	unsigned int ebo = createEbo(indices, vertexCount);

	double previousTime = glfwGetTime();
	int frameCount = 0;

	glEnable(GL_DEPTH_TEST);    // Enable the depth buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Ask for nicest perspective correction
	glFrontFace(GL_CW);

	//glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window)) {

		double currentTime = glfwGetTime();
		frameCount++;
		// If a second has passed.
		if (currentTime - previousTime >= 1.0) {
			std::cout << "\rfps: " << frameCount << std::flush;

			frameCount = 0;
			previousTime = currentTime;
		}

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double timeValue = glfwGetTime();
		float r = (sin(timeValue) / 2.0f) + 0.5f;
		float g = (cos(timeValue) / 2.0f) + 0.5f;
		float b = (sin(timeValue) / 2.0f) + 0.5f;
		shader.setFloat("ourColor", r, g, b);
		shader.setFloat("offset", timeValue);
		shader.setFloat("rotation", timeValue);
		shader.use();
		glBindTexture(GL_TEXTURE_2D, texture);
		drawSomething(vao, ebo, indexCount);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawSomething(unsigned int vao, unsigned int ebo, int indexCount) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
}

unsigned int generateObject() {
	unsigned int object;
	glGenBuffers(1, &object);
	return object;
}

unsigned int createVao(unsigned int vbo, float* vertices, unsigned int size) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return vao;
}

unsigned int createEbo(unsigned int* indices, unsigned int size) {
	unsigned int ebo = generateObject(); // Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	return ebo;
}