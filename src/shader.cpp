#include "shader.h"

namespace shader
{
	Shader::Shader()
	{
	}

	Shader::~Shader()
	{
		glDeleteShader(handle);
	}

	Shader::Shader(GLenum type, const std::string& source)
	{
		handle = glCreateShader(type);
		const char* cSource = source.c_str();
		glShaderSource(handle, 1, &cSource, nullptr);
		glCompileShader(handle);
		int success;
		char infoLog[512];
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(handle, 512, NULL, infoLog);
			throw std::runtime_error("shader compilation failed: " + std::string(infoLog));
		}
	}

	Shader::Shader(Shader&& other) noexcept
	{
		handle = other.handle;
		other.handle = 0;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		handle = other.handle;
		other.handle = 0;
		return *this;
	}

	const GLuint& Shader::operator*() const noexcept
	{
		return handle;
	}

	VertexShader::VertexShader(const std::string& source)
		: Shader(GL_VERTEX_SHADER, source)
	{
	}

	FragmentShader::FragmentShader(const std::string& source)
		: Shader(GL_FRAGMENT_SHADER, source)
	{
	}

	Program::Program()
	{
	}

	Program::Program(const std::initializer_list<std::reference_wrapper<Shader>>& shaders)
	{
		handle = glCreateProgram();
		for (const Shader& shader : shaders)
		{
			glAttachShader(handle, *shader);
		}
		glLinkProgram(handle);
		int success;
		char infoLog[512];
		glGetProgramiv(handle, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(handle, 512, nullptr, infoLog);
			throw std::runtime_error("shader program linking failed: " + std::string(infoLog));
		}
	}

	Program::~Program()
	{
		glDeleteProgram(handle);
	}

	Program::Program(Program&& other) noexcept
	{
		handle = other.handle;
		other.handle = 0;
	}

	Program& Program::operator=(Program&& other) noexcept
	{
		handle = other.handle;
		other.handle = 0;
		return *this;
	}

	const GLuint& Program::operator*() const noexcept
	{
		return handle;
	}
}
