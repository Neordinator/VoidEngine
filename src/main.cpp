// СТОРОННИЕ
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ИЗ КОРОБКИ
#include <iostream>

// СОБСТВЕННЫЕ
//#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"

GLfloat point[] = {
	0.0f, 50.0f, 0.0f,
	50.0f, -50.0f, 0.0f,
	-50.0f, -50.0f, 0.0f
};
GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};
GLfloat texCoord[] = {
	0.5f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};

glm::vec2 windowSize(640, 480);

/*int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;*/

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
	/*WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);*/
	windowSize.x = width;
	windowSize.y = height;
	glViewport(0, 0, windowSize.x, windowSize.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindow, GL_TRUE);
	}
}

int main(int argc, char** argv)
{
	// Initialize the Library
	if (!glfwInit())
	{
		std::cout << "glfwInit failed!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	//GLFWwindow* pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Void 0.01", nullptr, nullptr);
	GLFWwindow* pWindow = glfwCreateWindow(windowSize.x, windowSize.y, "Void 0.01", nullptr, nullptr);
	if (!pWindow)
	{
		std::cout << "glfwCreateWindow failed!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
	glfwSetKeyCallback(pWindow, glfwKeyCallback);

	// Make the window's context current
	glfwMakeContextCurrent(pWindow);

	if (!gladLoadGL())
	{
		std::cout << "Can not load GLAD!" << std::endl;
	}
	std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	//std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;

	glClearColor(1, 1, 0, 1);

	{
		ResourceManager resourceManager(argv[0]);

		auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
		if (!pDefaultShaderProgram)
		{
			std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
			return -1;
		}

		auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

		GLuint points_vbo = 0;
		glGenBuffers(1, &points_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

		GLuint colors_vbo = 0;
		glGenBuffers(1, &colors_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		GLuint texCoord_vbo = 0;
		glGenBuffers(1, &texCoord_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		pDefaultShaderProgram->use();
		pDefaultShaderProgram->setInt("tex", 0);

		glm::mat4 modelMatrix_1 = glm::mat4(1.f);
		modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.0f, 0.0f));

		glm::mat4 modelMatrix_2 = glm::mat4(1.f);
		modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.0f, 0.0f));

		glm::mat4 projectionMatrix = glm::ortho(0.0f, windowSize.x, 0.0f, windowSize.y, -100.0f, 100.0f);

		pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

		// Loop until the user closes the window
		while (!glfwWindowShouldClose(pWindow))
		{
			// Render here
			glClear(GL_COLOR_BUFFER_BIT);

			pDefaultShaderProgram->use();
			glBindVertexArray(vao);
			tex->bind();

			pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			// Swap front and back buffers
			glfwSwapBuffers(pWindow);

			// Poll for and process events
			glfwPollEvents();
		}
	}//область видимости для вызова деструктора ResourceManager перед уничтожением контекста OpenGL
	glfwTerminate();
	return 0;
}
