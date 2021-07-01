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

void Renderer2D::clear(Vect4<float> color) const
{
    GLCALL(glClearColor(color.r, color.g, color.b, color.a));
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::draw(const struct VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    //shader.bind(); uncomment when can send uniforms
    va.bind();
    ib.bind();

    GLCALL(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr)); // nullptr because ibo already bound to gpu

#   ifdef _DEBUG
    shader.unbind();
    va.unbind();
    ib.unbind();
#   endif
}
