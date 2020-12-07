#pragma once

class IndexBuffer {

private:

	unsigned int renderer_ID;
	unsigned int m_count;

public:

	IndexBuffer(void* data, unsigned int count);
	~IndexBuffer();

	void bind();
	void unbind();

	void selectIndices( unsigned int* data, unsigned int count);

	void deleteBuffer();

	inline unsigned int getCount() { return m_count; }
};

