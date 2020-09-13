#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H_

#include <cstdint>

#include "glad/gl.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void bind() noexcept;
	void unbind() noexcept;

	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	VertexArray(VertexArray& other) = delete;
	VertexArray& operator=(VertexArray& other) = delete;

private:
	uint32_t handle;
};

#endif // !VERTEX_ARRAY_H_
