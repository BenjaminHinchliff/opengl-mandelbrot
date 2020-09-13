#include <iostream>
#include <array>
#include <fstream>
#include <sstream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "shader.h"

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

class GlfwInitializer
{
public:
	GlfwInitializer();
	~GlfwInitializer();
};

GlfwInitializer::GlfwInitializer()
{
	if (!glfwInit())
	{
		throw std::runtime_error("failed to initialize glfw");
	}
}

GlfwInitializer::~GlfwInitializer()
{
	glfwTerminate();
}

void processInput(GLFWwindow* window);

int main()
{
	GlfwInitializer glfwContext;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mandelbrot Renderer", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "failed to create window" << '\n';
		return 1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress)))
	{
		std::cerr << "failed to init GLAD2" << '\n';
		return 1;
	}

	glfwSetFramebufferSizeCallback(
		window,
		[](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	);

	shader::Program program;
	try
	{
		std::ifstream vertexFile("../shaders/basic.vert");
		std::stringstream vertexStream;
		vertexStream << vertexFile.rdbuf();
		std::string vertexSource(vertexStream.str());
		shader::VertexShader vertexShader(vertexSource);

		std::ifstream fragmentFile("../shaders/mandelbrot.frag");
		std::stringstream fragmentStream;
		fragmentStream << fragmentFile.rdbuf();
		std::string fragmentSource(fragmentStream.str());
		shader::FragmentShader fragmentShader(fragmentSource);

		program = shader::Program({ vertexShader, fragmentShader });
	}
	catch (const std::runtime_error &e)
	{
		std::cout << "failed to create program: " << e.what() << '\n';
	}

	glUseProgram(*program);
	glUniform2f(glGetUniformLocation(*program, "resolution"), SCR_WIDTH, SCR_HEIGHT);
	glUniform1i(glGetUniformLocation(*program, "max_iter"), 50);
	glUseProgram(0);

	std::array<float, 8> vertices{
		 1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f,
	};

	std::array<float, 8> texCoords{
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
	};

	std::array<uint32_t, 6> indices{
		0, 1, 3,
		1, 2, 3
	};

	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint32_t TBO;
	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint32_t EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.5f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(*program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
