#pragma once
#include <GL/glew.h>

#define DEFAULT_SHADER_FILE "res/shaders/basic.shader"
// relative to solution directory

#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCALL(x) GLClearError(); x; ASSERT(GLLogCall(__FILE__, #x, __LINE__))
// #x will turn x into a string (const char*) e.g. x = glDraw(), #x = "glDraw()"


// error handling with glGetError() (legacy way)
void GLClearError();
bool GLLogCall(const char* file, const char* func, int line);