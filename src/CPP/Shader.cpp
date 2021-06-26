#include "Shader.h"
#include "GL/glew.h"
#include "Renderer.h"

#include <fstream>
#include <iostream>

struct ShaderSources
{
	string vert, frag;
};


Shader::Shader(const string& rel_path)
	: shader_buf(0), path(rel_path)
{
    this->parse_file();
    this->shader_buf = this->create_shaders();
    this->bind();
}
Shader::~Shader()
{
	GLCALL(glDeleteProgram(this->shader_buf));
}


void Shader::bind() const
{
	GLCALL(glUseProgram(this->shader_buf));
}
void Shader::unbind() const
{
    GLCALL(glUseProgram(0));
}


unsigned int Shader::compile_shader(unsigned int type, const string& _src)
{
    GLCALL(unsigned int shader_id = glCreateShader(type));
    const char* src = _src.c_str();
    GLCALL(glShaderSource(shader_id, 1, &src, nullptr));
    GLCALL(glCompileShader(shader_id));

    // ERROR HANDLING
    int result;                                           // to store the possible error code
    GLCALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result)); // returns 0 if there is an error while COMPILING shader source

    if (result == GL_FALSE)
    {
        int len;
        GLCALL(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len));                            // length of error message
        char* msg = (char*)_malloca(len * sizeof(char));                                       // allocate mem for eror message
        GLCALL(glGetShaderInfoLog(shader_id, len, &len, msg));

        std::cout << "Error compiling shader type <";
        std::cout << (type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : "GL_FRAGMENT_SHADER"); // put shader type in string form using ternary operator
        std::cout << ">:\n" << msg << std::endl;                                               // print error message

        GLCALL(glDeleteShader(shader_id));
        return 0;
    }

    return shader_id;
}

unsigned int Shader::create_shaders()
{
    // create shaders
    GLCALL(unsigned int prgrm = glCreateProgram());
    unsigned int vs = Shader::compile_shader(GL_VERTEX_SHADER, this->vert_src);
    unsigned int fs = Shader::compile_shader(GL_FRAGMENT_SHADER, this->frag_src);

    // combine shaders
    GLCALL(glAttachShader(prgrm, vs));
    GLCALL(glAttachShader(prgrm, fs));
    GLCALL(glLinkProgram(prgrm));
    GLCALL(glValidateProgram(prgrm));

    // delete shaders
#   ifdef NDEBUG
        GLCALL(glDetachShader(prgrm, vs)); // remove for debugging
        GLCALL(glDetachShader(prgrm, fs)); // remove for debugging
#   endif

    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return prgrm;
}


void Shader::parse_file()
{
    // FILE* file = fopen(path.c_str(), "r"); // c-style open file

    std::ifstream shader_file{ this->path };
    string strs[2] = { this->vert_src, this->frag_src };

    if (shader_file.is_open())
    {
        std::cout << "Reading file <" << this->path << "> ... ";

        enum class ShaderType
        {
            NONE = -1, VERTEX, FRAGMENT
        };

        string line;
        ShaderType shader_type = ShaderType::VERTEX; // default type is vertex

        while (getline(shader_file, line))
        {
            //// line comment
            //if (line[0] == '/' && line[1] == '/')
            //    continue;

            // shader type identifier
            if (line.find("#shader") != string::npos)
                if (line.find("vertex") != string::npos)
                    shader_type = ShaderType::VERTEX;

                else if (line.find("fragment") != string::npos)
                    shader_type = ShaderType::FRAGMENT;
                else;

            // line is appended to buffers
            else
                switch (shader_type)
                {
                case ShaderType::VERTEX:
                    this->vert_src += (line + '\n');
                    break;
                case ShaderType::FRAGMENT:
                    this->frag_src += (line + '\n');
                }
        }

        std::cout << "Done parsing file <" << path << ">\n";
        shader_file.close();
    }
    else
        std::cout << "Error opening file <" << path << ">\n";
}


void Shader::set_uniform(const string& name, Vect4<float> v)
{
    GLCALL(glUniform4f( // pass vec4 to uniform in fragment shader
        this->get_uniform_location(name),
        v.x, v.y, v.z, v.w
    ));
}
void Shader::set_uniform(const string& name, Vect3<float> v)
{
    GLCALL(glUniform3f(
        this->get_uniform_location(name),
        v.x, v.y, v.z
    ));
}
void Shader::set_uniform(const string& name, Vect2<float> v)
{
    GLCALL(glUniform2f(
        this->get_uniform_location(name),
        v.x, v.y
    ));
}
void Shader::set_uniform(const string& name, float v)
{
    GLCALL(glUniform1f(this->get_uniform_location(name), v));
}
void Shader::set_uniform(const string& name, double v)
{
    this->set_uniform(name, (float)v);
}
void Shader::set_uniform(const string& name, int v)
{
    GLCALL(glUniform1i(this->get_uniform_location(name), v));
}


int Shader::get_uniform_location(const string& name)
{
    if (this->uniforms_cache.find(name) != this->uniforms_cache.end())
        return this->uniforms_cache[name];

    int u_loc;
    GLCALL(u_loc = glGetUniformLocation(this->shader_buf, name.c_str()));

    if (u_loc == -1)
        std::cout << "WARNING!  --  Uniform <" << name << "> not in use.\n";

    this->uniforms_cache[name] = u_loc;
    return u_loc;
}