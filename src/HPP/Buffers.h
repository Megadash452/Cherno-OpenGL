#pragma once
#include <vector>
#include <GL/glew.h>

#include "Renderer.h"


struct VertexBuffer
{
	// Think of buffers as the renderer

	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

private:
	unsigned int buf_id;
};


struct LayoutElement
{
	unsigned int type, count;
	unsigned char normalized;

	static unsigned int size_of_type(unsigned int type);
};

struct VertexBuffer_Layout
{
	VertexBuffer_Layout()
		: stride(0)
	{}

	template<typename T>
	void push(unsigned int count)
	{
		static_assert(false);
	}

	template <>
	void push<float>(unsigned int count)
	{
		this->elements.push_back(LayoutElement{ GL_FLOAT, count, GL_FALSE });
		this->stride += count * LayoutElement::size_of_type(GL_FLOAT);
	}
	template <>
	void push<unsigned int>(unsigned int count)
	{
		this->elements.push_back(LayoutElement{ GL_UNSIGNED_INT, count, GL_FALSE });
		this->stride += count * LayoutElement::size_of_type(GL_UNSIGNED_INT);
	}
	template <>
	void push<unsigned char>(unsigned int count)
	{
		this->elements.push_back(LayoutElement{ GL_UNSIGNED_BYTE, count, GL_TRUE });
		this->stride += count * LayoutElement::size_of_type(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<LayoutElement>& get_elements() const
	{ return this->elements; }

	inline unsigned int get_stride() const
	{return this->stride;}

private:
	std::vector<LayoutElement> elements;
	unsigned int stride;

};


struct IndexBuffer
{
	IndexBuffer(const unsigned int* data, unsigned int _count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int get_count() const
	{ return this->count; }

private:
	unsigned int buf_id;
	unsigned int count;
};


struct VertexArray
{
	VertexArray();
	~VertexArray();

	void bind() const;
	void unbind() const;

	void add_buf(const VertexBuffer& vert_buf, const VertexBuffer_Layout& layout);

private:
	unsigned int buf_id;
};