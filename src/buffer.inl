#include "buffer.h"

template<typename T>
Buffer<T>::Buffer()
{
}

template<typename T>
Buffer<T>::Buffer(GLenum typeParam, const std::vector<T>& dataParam, GLenum usage)
	: type(typeParam), data(dataParam)
{
	glGenBuffers(1, &handle);
	glBindBuffer(type, handle);
	glBufferData(type, sizeof(data), data.data(), usage);
	glBindBuffer(type, 0);
}

template<typename T>
Buffer<T>::~Buffer()
{
	glDeleteBuffers(1, &handle);
}

template<typename T>
void Buffer<T>::bind() noexcept
{
	glBindBuffer(type, handle);
}

template<typename T>
void Buffer<T>::unbind() noexcept
{
	glBindBuffer(type, 0);
}

template<typename T>
Buffer<T>::Buffer(Buffer&& other) noexcept
{
	type = other.type;
	data = std::move(other.data);
	handle = other.handle;
	other.handle = 0;
}

template<typename T>
Buffer<T>& Buffer<T>::operator=(Buffer&& other) noexcept
{
	type = other.type;
	data = std::move(other.data);
	handle = other.handle;
	other.handle = 0;
	return *this
}
