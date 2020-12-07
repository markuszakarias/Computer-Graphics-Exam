#include "VertexArray.h"

/**
*   The VertexArray class creates a VAO that binds a mesh. It is used together with the 
*	VertexBuffer and VertexBufferLayout classes. 
*
*   @name VertexArray.cpp
*   @author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*	Constructor for a VertexArray.
*
*   @see bind()
*/
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &rendererID); //Generating a vertex array for the array object
	bind();
}

/**
*	Destructor for a VertexArray.
*
*/
VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &rendererID);
}

/**
*	Adds a buffer with the VertexBuffer and VertexBufferLayout. Binds both the VertexArray
*	and VertexBuffer, and uses VertexBufferLayout to the AttribArray of the object.
*
*   @param vb		- Takes in a VertexBuffer
*   @param layout	- Takes in a VertexBufferLayout
* 
*	@see bind()
*/
void VertexArray::addBuffer(VertexBuffer& vb, VertexBufferLayout& layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized,
			layout.getStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

/**
*	The attrib arrays that are needed for the instance rendering of the pellets. Currently
*	running from here to clean up code. 
*
*/
void VertexArray::addBufferDivisor()
{
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glVertexAttribDivisor(3, 1);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glVertexAttribDivisor(4, 1);

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribDivisor(5, 1);

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(6, 1);
}

/**
*	Binds the VertexArray
*
*/
void VertexArray::bind()
{
	glBindVertexArray(rendererID);
}

/**
*	Unbinds the VertexArray
*
*/
void VertexArray::unbind()
{
	glBindVertexArray(0);
}

/**
*	Changes the data of the VertexArray
*
*   @param VBO	- The VertexBuffer that gets updated
*   @param data	- The data of the updated buffer
*   @param size	- The size of the updated buffer
*/
void VertexArray::changeData(std::shared_ptr<VertexBuffer>& VBO, void* data, unsigned int size)
{
	bind();
	VBO->bind();
	VBO->updateBuffer(data, size);
}
