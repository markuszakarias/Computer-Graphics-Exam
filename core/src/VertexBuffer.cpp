#include <GL/glew.h>
#include "VertexBuffer.h"

/**
*	The VertexBuffer takes in the vertices and generates buffers for them.
*
*  @name VertexBuffer.cpp
*  @author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*	Constructor for VertexBuffer.
*
*   @param data	- The data that gets added to the buffer.
*   @param size	- The size of the data that gets added to the buffer.
* 
*	@see bind()
*/
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &renderer_ID); //generating a buffer for the vertex buffer object
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

/**
*	Destructor for a VertexBuffer.
*
*/
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &renderer_ID);
}

/**
*	Binds the VertexBuffer.
*
*/
void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, renderer_ID);
}

/**
*	Unbinds the VertexBuffer.
*
*/
void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
*	Updates the buffer with new data and size.
* 
*	@param data	- The data to get updated
*	@param size	- The size of the data to get updated
*/
void VertexBuffer::updateBuffer(const void* data, unsigned int size)
{
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}