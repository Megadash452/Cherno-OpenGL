#include "Buffers.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCALL(glGenBuffers(1, &this->rendererID));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->rendererID)); // bind buffer to gpu
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    // the positions are static. use GL_DYNAMIC_DRAW for positions that can change
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &this->rendererID));
}

void VertexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->rendererID));
}

void VertexBuffer::unbind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}



IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int _count)
    : count(_count)
{
    ASSERT(sizeof(GLuint) == sizeof(unsigned int));
    // in some systems these may not be the same (very rare)

    GLCALL(glGenBuffers(1, &this->rendererID));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->rendererID)); // bind buffer to gpu
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &this->rendererID));
}

void IndexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->rendererID));
}

void IndexBuffer::unbind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
