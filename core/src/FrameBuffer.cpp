#include "FrameBuffer.h"

/**
*	The buffers used to create and bind the minimap view of the game. It generates a frame buffer, binds it,
*	and renders the scene to a texture. This texture is later applied to a quad and placed at the top right
*	corner of the window.
*
*   @name FrameBuffer.cpp
*   @author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*   Constructor for the frame buffer object.
*
*/
FrameBuffer::FrameBuffer()
	: frameBuffer(0), texColorBuffer(0), renderObjectBuffer(0) {

}

/**
*   Destructor for the frame buffer object.
*
*/
FrameBuffer::~FrameBuffer() {

}

/**
*   Generates and binds the frameBuffer.
* 
*	@see bind().
*
*/
void FrameBuffer::generateFB() {

	glGenFramebuffers(1, &frameBuffer);
	bind();
}

/**
*   Generates and binds the texture color buffer.
* 
*	@param width	-	The width of the buffer.
*	@param height	-	The height of the buffer.
* 
*	@see bindTBO().
*
*/
void FrameBuffer::generateTBO(int width, int height) {

	glGenTextures(1, &texColorBuffer);
	bindTBO();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
}

/**
*   Generates and binds the render buffer object.
*
*	@param width	-	The width of the buffer.
*	@param height	-	The height of the buffer.
*
*	@see bindRBO().
*
*/
void FrameBuffer::generateRBO(int width, int height) {

	glGenRenderbuffers(1, &renderObjectBuffer);
	bindRBO();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single render buffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderObjectBuffer); // now actually attach it

}

/**
*   Binds the frame buffer.
*
*/
void FrameBuffer::bind() {

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

/**
*   Binds the texture color object.
*
*/
void FrameBuffer::bindTBO() {

	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
}

/**
*   Binds the render buffer object.
*
*/
void FrameBuffer::bindRBO() {

	glBindRenderbuffer(GL_RENDERBUFFER, renderObjectBuffer);
}

/**
*   Unbinds the frame buffer.
*
*/
void FrameBuffer::unbind() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
*   Checks to see if the frame buffer is complete.
*
*/
void FrameBuffer::checkStatus() {

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Error:  Frame buffer is not completed, missing components!" << std::endl;
}
