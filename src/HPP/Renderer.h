#pragma once
#include "Buffers.h"
#include "Shader.h"
#include <GL/glew.h>

#include <unordered_map>
#include <vector>

#define DEFAULT_SHADER_FILE "res/shaders/basic.shader"
// relative to solution directory

#define ASSERT(x) if (!(x)) __debugbreak()
#if _DEBUG
#	define GLCALL(x) GLClearError(); x; ASSERT(GLLogCall(__FILE__, #x, __LINE__))
// #x will turn x into a string (const char*) e.g. x = glDraw(), #x = "glDraw()"
#elif
#	define GLCALL(x) x;
#endif


// error handling with glGetError() (legacy way)
void GLClearError();
bool GLLogCall(const char* file, const char* func, int line);

struct Renderer2D
{
	void clear() const;
	//void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void draw(Texture t) const;

private:
	VertexArray va;
	VertexBuffer vb;
	VertexBuffer_Layout vl;
	std::vector<Shader> shaders;
	IndexBuffer ib;

	glm::mat4 proj_matrix;
	glm::mat4 view_matrix;
};