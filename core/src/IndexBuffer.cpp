#include <iostream>
#include <GL/glew.h>

#include "IndexBuffer.h"

/**
*  IndexBuffer abstracts the code required to deal with index buffers.
*  This makes handling them in the program much easier and cleaner in
*  terms of code length
* 
*  Example can be seen in any object creation related to the game.
*
*  @name IndexBuffer.cpp
*  @author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*	Constructor for the IndexBuffer. Generates a buffer for the object being created.
*
*	@param data 	- The data that is sent to the buffer.
*	@param count	- The number of elements that is sent to the buffer.
* 
*/
IndexBuffer::IndexBuffer(void* data, unsigned int count)
	: m_count(count) {

	glGenBuffers(1, &renderer_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

/**
*	Destructor for the IndexBuffer.
* 
*	@see deleteBuffer()
*
*/
IndexBuffer::~IndexBuffer() {

	deleteBuffer();
}

/**
*	Binds the buffer.
*
*/
void IndexBuffer::bind() {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_ID);
}

/**
*	Unbinds the buffer.
*
*/
void IndexBuffer::unbind() {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/**
*	Updates the buffer, with a new set of data, and re-binds it.
* 
*	@param data 	- The data that is sent to the buffer.
*	@param count	- The number of elements that is sent to the buffer.
*
*	@see bind()
*/
void IndexBuffer::selectIndices(unsigned int* data, unsigned int count) {

	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

/**
*	Deletes the buffer.
*
*/
void IndexBuffer::deleteBuffer() {

	glDeleteBuffers(1, &renderer_ID);
}
