#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"
#include "Renderer.h"

unsigned int CompileShader(unsigned int type, const string& _src)
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

unsigned int CreateShader(const string& vertex_shader, const string& fragment_shader)
{
    // create shaders
    GLCALL(unsigned int prgrm = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

    // combine shaders
    GLCALL(glAttachShader(prgrm, vs));
    GLCALL(glAttachShader(prgrm, fs));
    GLCALL(glLinkProgram(prgrm));
    GLCALL(glValidateProgram(prgrm));

    // delete shaders
#   if NDEBUG
    GLCALL(glDetachShader(prgrm, vs)); // remove for debugging
    GLCALL(glDetachShader(prgrm, fs)); // remove for debugging
#   endif
    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return prgrm;
}

ShaderSources parse_shader_file(const string& path)
{
    // FILE* file = fopen(path.c_str(), "r"); // c-style open file

    std::ifstream shader_file{ path };
    std::stringstream buf[2];

    if (shader_file.is_open())
    {
        std::cout << "Reading file <" << path << "> ... ";

        enum class ShaderType
        {
            NONE = -1, VERTEX, FRAGMENT
        };

        string line;
        ShaderType shader_type = ShaderType::VERTEX; // default type is vertex

        while (getline(shader_file, line))
        {
            // line comment
            if (line[0] == '/' && line[1] == '/')
                continue;

            // shader type identifier
            else if (line.find("#shader") != string::npos)
                if (line.find("vertex") != string::npos)
                    shader_type = ShaderType::VERTEX;
                else if (line.find("fragment") != string::npos)
                    shader_type = ShaderType::FRAGMENT;
                else;

            // line is appended to buffers
            else
                buf[(int)shader_type] << line << '\n';
        }

        std::cout << "Done reading file <" << path << ">\n";
        shader_file.close();
    }
    else
        std::cout << "Error opening file <" << path << ">\n";

    return { buf[0].str(), buf[1].str() };
}