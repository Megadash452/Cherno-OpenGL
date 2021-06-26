#include "Buffers.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCALL(glGenBuffers(1, &this->buf_id));
    this->bind();
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    // the positions are static. use GL_DYNAMIC_DRAW for positions that can change
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &this->buf_id));
}

void VertexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->buf_id)); // bind buffer to gpu
}

void VertexBuffer::unbind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


unsigned int LayoutElement::size_of_type(unsigned int type)
{
    switch (type)
    {
    case GL_FLOAT:
        return 4;
    case GL_UNSIGNED_INT:
        return 4;
    case GL_UNSIGNED_BYTE:
        return 1;
    }

    ASSERT(false);
    return 0;
}


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int _count)
    : count(_count)
{
    ASSERT(sizeof(GLuint) == sizeof(unsigned int));
    // in some systems these may not be the same (very rare)

    GLCALL(glGenBuffers(1, &this->buf_id));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buf_id)); // bind buffer to gpu
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &this->buf_id));
}

void IndexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->buf_id)); // Element array buffer??
}

void IndexBuffer::unbind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Element array buffer??
}



VertexArray::VertexArray()
{
    GLCALL(glGenVertexArrays(1, &this->buf_id));
    this->bind();
}

VertexArray::~VertexArray()
{
    GLCALL(glDeleteVertexArrays(1, &this->buf_id));
}

void VertexArray::bind() const
{
    GLCALL(glBindVertexArray(this->buf_id));
}

void VertexArray::unbind() const
{
    GLCALL(glBindVertexArray(0));
}

void VertexArray::add_buf(const VertexBuffer& vert_buf, const VertexBuffer_Layout& layout)
{
    this->bind();
    vert_buf.bind();

    const std::vector<LayoutElement>& elements = layout.get_elements();

    unsigned int offset = 0;
    for (unsigned int i=0; i < elements.size(); i++)
    {
        const LayoutElement& element = elements[i];
        
        GLCALL(glEnableVertexAttribArray(i));
        GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get_stride(), (const void*)offset)); // tell GL the layout of VERTEX BUFFER

        offset += element.count * LayoutElement::size_of_type(element.type);
    }
}