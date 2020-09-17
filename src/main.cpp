#include <iostream>
#include <array>
#include <fstream>
#include <sstream>
#include <functional>

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "shader.h"
#include "buffer.h"
#include "vertex_array.h"
#include "texture.h"

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;
constexpr uint32_t IMG_WIDTH = 1600;
constexpr uint32_t IMG_HEIGHT = 1200;

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

struct WindowData
{
	std::function<void()> drawFn;
	shader::Program& program;
	int resolutionLoc;
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

	std::function<void()> draw = [&]() {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		texture.bind();
		program.use();
		VAO.bind();
		EBO.bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	};

	WindowData resolution{ draw, program, programLoc };

	glfwSetWindowUserPointer(window, &resolution);
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	//uint32_t fbo;
	//glGenFramebuffers(1, &fbo);
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//uint32_t renderedTex;
	//glGenTextures(1, &renderedTex);
	//glBindTexture(GL_TEXTURE_2D, renderedTex);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMG_WIDTH, IMG_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTex, 0);
	//
	//GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	//glDrawBuffers(1, DrawBuffers);

	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	std::cerr << "something went wrong the framebuffer" << '\n';
	//	return 1;
	//}

	//glViewport(0, 0, IMG_WIDTH, IMG_HEIGHT);
	//draw();

	//std::vector<uint8_t> data;
	//data.reserve(IMG_WIDTH * IMG_HEIGHT * 4);
	//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	//stbi_write_png("mandelbrot.png", IMG_WIDTH, IMG_HEIGHT, 4, data.data(), IMG_WIDTH * 4);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		draw();
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
	WindowData& uniform = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
	glViewport(0, 0, width, height);
	glUseProgram(*uniform.program);
	glUniform2f(uniform.resolutionLoc, static_cast<float>(width), static_cast<float>(height));
	glUseProgram(0);
	uniform.drawFn();
	glfwSwapBuffers(window);
}
