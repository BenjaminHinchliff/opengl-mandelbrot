#include "texture.h"
#include "stb/stb_image.h"

Texture::Texture()
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

Texture1D::Texture1D() : Texture()
{
}

Texture1D::Texture1D(const std::string& filepath, GLenum wrapS, GLenum minFilter, GLenum magFilter) : Texture1D()
{
	int width;
	int height;
	int nrChannels;
	stbi_set_flip_vertically_on_load(true);
	std::unique_ptr<uint8_t, decltype(&stbi_image_free)> data(stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0), stbi_image_free);
	if (!data)
	{
		throw std::runtime_error("failed to load texture");
	}

	bind();
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, magFilter);

	GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA;
	glTexImage1D(GL_TEXTURE_1D, 0, format, width, 0, format, GL_UNSIGNED_BYTE, data.get());
	glGenerateMipmap(GL_TEXTURE_1D);
	unbind();
}

 GLuint Texture::operator*()
{
	return texture;
}


void Texture1D::bind()
{
	glBindTexture(GL_TEXTURE_1D, texture);
}

void Texture1D::unbind()
{
	glBindTexture(GL_TEXTURE_1D, 0);
}
