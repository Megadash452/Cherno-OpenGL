#pragma once
#include <string>
using std::string;

struct Texture
{
	Texture(const string& file_path);
	~Texture();

	void bind(unsigned int slot = 0);
	void unbind();

	void get_width() const;
	void get_height() const;

private:
	unsigned int texture_buf;
	unsigned char* local_buf;
	string file_path;

	int width, height, BPP;
};