#include "Texture.h"

Texture::Texture(const string& file_path)
	: texture_buf(0), file_path(file_path), local_buf(nullptr), width(0), height(0), BPP(0)
{

}