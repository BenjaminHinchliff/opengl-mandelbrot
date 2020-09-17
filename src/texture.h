#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <stdexcept>
#include <memory>

#include "glad/gl.h"

class Texture
{
public:
	Texture();
	virtual ~Texture();

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual GLuint operator*();

protected:
	GLuint texture = 0;
};

class Texture1D : public Texture
{
public:
	Texture1D();
	Texture1D(const std::string& filepath, GLenum wrapS = GL_REPEAT, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR);

	virtual void bind();
	virtual void unbind();
};

#endif // !TEXTURE_H_
