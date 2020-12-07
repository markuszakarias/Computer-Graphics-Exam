#pragma once

#include <memory>
#include "VertexArray.h"
#include "IndexBuffer.h"

class Renderer
{
private:

public:

	void drawArrays(std::shared_ptr<VertexArray>& va);
	void drawElements(std::shared_ptr<VertexArray>& va, std::shared_ptr<IndexBuffer>& ib);
	void drawInstanced(std::shared_ptr<VertexArray>& va, std::shared_ptr<IndexBuffer>& ib, int numInstanced);
	
	void enableDepth();
	void disableDepth();
	void clearBuffer();
	void clear(float f0, float f1, float f2, float f3) const;

};
