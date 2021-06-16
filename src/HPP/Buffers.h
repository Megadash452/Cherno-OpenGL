#pragma once

struct VertexBuffer
{
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

private:
	unsigned int rendererID;
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
	unsigned int rendererID;
	unsigned int count;
};
