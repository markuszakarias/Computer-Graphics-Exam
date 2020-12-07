#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {

private:

	unsigned int rendererID;

public:

	VertexArray();
	~VertexArray();

	void addBuffer( VertexBuffer& vb, VertexBufferLayout& layout);
	void addBufferDivisor();

	void bind();
	void unbind();

	void changeData(std::shared_ptr<VertexBuffer>& VBO, void* data, unsigned int size);

	inline unsigned int getVertexArray() { return rendererID; }

};

