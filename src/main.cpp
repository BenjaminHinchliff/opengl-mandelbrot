#include <iostream>
#include <array>
#include <fstream>
#include <sstream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "shader.h"
#include "buffer.h"
#include "vertex_array.h"
#include "texture.h"

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

struct ResolutionUniform
{
	shader::Program& program;
	int loc;
};

void processInput(GLFWwindow* window);
void resizeCallback(GLFWwindow* window, int width, int height);

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
	int programLoc = glGetUniformLocation(*program, "resolution");
	glUniform2f(programLoc, SCR_WIDTH, SCR_HEIGHT);
	glUniform1i(glGetUniformLocation(*program, "max_iter"), 50);
	glUseProgram(0);

	ResolutionUniform resolution{ program, programLoc };

	glfwSetWindowUserPointer(window, &resolution);
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	std::vector<float> vertices{
		 1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f,
	};

	std::vector<float> texCoords{
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
	};

	std::vector<uint32_t> indices{
		0, 1, 3,
		1, 2, 3
	};

	VertexArray VAO;
	VAO.bind();

	Buffer<float> VBO(GL_ARRAY_BUFFER, vertices);
	VBO.bind();
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	VBO.unbind();

	Buffer<float> TBO(GL_ARRAY_BUFFER, texCoords);
	TBO.bind();
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	TBO.unbind();

	VAO.unbind();

	Buffer<uint32_t> EBO(GL_ELEMENT_ARRAY_BUFFER, indices);

	Texture1D texture("../textures/palette.png");
	
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		texture.bind();
		program.use();
		VAO.bind();
		EBO.bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

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

void resizeCallback(GLFWwindow* window, int width, int height)
{
	ResolutionUniform& uniform = *static_cast<ResolutionUniform*>(glfwGetWindowUserPointer(window));
	glViewport(0, 0, width, height);
	glUseProgram(*uniform.program);
	glUniform2f(uniform.loc, static_cast<float>(width), static_cast<float>(height));
	glUseProgram(0);
}
