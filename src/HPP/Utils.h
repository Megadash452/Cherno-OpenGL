#pragma once
#include <string>
using std::string;


struct ShaderSources
{
    string vert, frag;
};

unsigned int CompileShader(unsigned int type, const string& _src);
unsigned int CreateShader(const string& vertex_shader, const string& fragment_shader);

ShaderSources parse_shader_file(const string& path);