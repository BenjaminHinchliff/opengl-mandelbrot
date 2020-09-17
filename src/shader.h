#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <stdexcept>
#include <vector>
#include <functional>

#include "glad/gl.h"

namespace shader
{
	class Shader
	{
	public:
		Shader();
		~Shader();
		Shader(GLenum type, const std::string& source);

		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

		Shader(Shader& other) = delete;
		Shader& operator=(Shader& other) = delete;

		const GLuint& operator*() const noexcept;
	private:
		GLuint handle = 0;
	};

	class VertexShader : public Shader
	{
	public:
		VertexShader(const std::string& source);
	};

	class FragmentShader : public Shader
	{
	public:
		FragmentShader(const std::string& source);
	};

	class Program
	{
	public:
		Program();
		Program(const std::initializer_list<std::reference_wrapper<Shader>>& shaders);
		~Program();

		Program(Program&& other) noexcept;
		Program& operator=(Program&& other) noexcept;

		Program(Program& other) = delete;
		Program& operator=(Program& other) = delete;

		void use() noexcept;
		void unuse() noexcept;

		const GLuint& operator*() const noexcept;
	private:
		GLuint handle = 0;
	};
}

#endif // !SHADER_H_
