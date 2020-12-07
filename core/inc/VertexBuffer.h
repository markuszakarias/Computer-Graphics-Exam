#pragma once

class VertexBuffer {

private:

	unsigned int renderer_ID;

public:

	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
	void updateBuffer(const void* data, unsigned int size);

};

