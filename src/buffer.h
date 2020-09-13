#ifndef BUFFER_H_
#define BUFFER_H_

#include "glad/gl.h"

template<typename T>
class Buffer
{
public:
	Buffer();
	Buffer(GLenum typeParam, const std::vector<T>& dataParam, GLenum usage = GL_STATIC_DRAW);
	~Buffer();

	void bind() const noexcept;
	void unbind() const noexcept;

	Buffer(Buffer&& other);
	Buffer& operator=(Buffer&& other);

	Buffer(Buffer& other) = delete;
	Buffer& operator=(Buffer& other) = delete;

private:
	GLenum type = 0;
	std::vector<T> data = {};
	uint32_t handle = 0;
};

#include "buffer.inl"
#endif // !BUFFER_H_
