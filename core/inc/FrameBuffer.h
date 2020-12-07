#pragma once

#include <cstddef>
#include <iostream>

#include "GL/glew.h"

class FrameBuffer {

private:

	unsigned int texColorBuffer;
	unsigned int frameBuffer;
	unsigned int renderObjectBuffer;

public:

	FrameBuffer();
	~FrameBuffer();

	void generateFB();
	void generateTBO(int width, int height);
	void generateRBO(int width, int height);

	void bind();
	void bindTBO();
	void bindRBO();
	void unbind();

	void checkStatus();
};