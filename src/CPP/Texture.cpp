#include "Texture.h"
#include "Renderer.h"
#include "../stb_image/stb_image.h"

Texture::Texture(const string& file_path)
	: texture_buf(0), file_path(file_path), img_buf(nullptr), width(0), height(0), BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	this->img_buf = stbi_load(this->file_path.c_str(), &this->width, &this->height, &this->BPP, 4); // 4->rgba

	GLCALL(glGenTextures(1, &this->texture_buf));
	GLCALL(glBindTexture(GL_TEXTURE_2D, this->texture_buf));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); // wrapping horizonatlly (x)
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); // wrapping vertically (y)

	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->img_buf));

	if (this->img_buf)
		stbi_image_free(this->img_buf);

	this->bind();
}

Texture::~Texture()
{
	GLCALL(glDeleteTextures(1, &this->texture_buf));
	/*if (this->img_buf)
		stbi_image_free(this->img_buf);*/
}

void Texture::bind(unsigned int slot)
{
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, this->texture_buf));
}

void Texture::unbind()
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::get_width() const
{
}

void Texture::get_height() const
{
}
