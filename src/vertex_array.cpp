#include "vertex_array.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &handle);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &handle);
}

void VertexArray::bind() noexcept
{
	glBindVertexArray(handle);
}

void VertexArray::unbind() noexcept
{
	glBindVertexArray(0);
}

VertexArray::VertexArray(VertexArray&& other) noexcept
{
	handle = other.handle;
	other.handle = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	handle = other.handle;
	other.handle = 0;
	return *this;
}

