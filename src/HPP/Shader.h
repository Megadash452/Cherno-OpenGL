#pragma once
#include <string>
#include <unordered_map>
using std::string;

#include "Structs.h"

struct Shader
{
	Shader(const string& rel_path);
	~Shader();

	void bind() const;
	void unbind() const;

	void set_uniform(const string& name, Vect4<float>);
	void set_uniform(const string& name, Vect3<float>);
	void set_uniform(const string& name, Vect2<float>);

private:
	unsigned int shader_buf;
	string path;
	string vert_src;
	string frag_src;

	std::unordered_map<string, int> uniforms_cache;

	void parse_file();
	unsigned int create_shaders();
	static unsigned int compile_shader(unsigned int type, const string& _src);
	int get_uniform_location(const string& name);
};