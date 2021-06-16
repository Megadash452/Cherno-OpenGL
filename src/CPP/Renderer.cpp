#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* file, const char* func, int line)
{
    while (GLenum err = glGetError())
    {
        std::cout << "[OpenGL Error] (code)<" << err << ">\n    @ \""
            << file << "\" > " << func
            << ",\n    @ line " << line << ":\n"
            << err /*get error in word form*/ << std::endl;
        return false;
    }
    return true;
}