#include <iostream>
#include "Renderer.h"

/**
*   The Renderer class lets us render the object onto the screen during runtime. We have two different kinds
*	of draw calls. The drawElements() is used for single objects, or objects that we dont find the need to be
*	instanced in our application. The drawInstanced is used for the Pellets in our application.
*
*   @name Renderer.cpp
*   @author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*   Draw call render for simple objects that don't have indices.
*
*   @param va - The VertexArray gets passed and bound
*/
void Renderer::drawArrays(std::shared_ptr<VertexArray>& va)
{
	va->bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

/**
*   Draw call render all our prepared buffers.
*
*   @param va - The VertexArray gets passed and bound
*   @param ib - The IndexBuffer gets passed and bound
*/
void Renderer::drawElements(std::shared_ptr<VertexArray>& va, std::shared_ptr<IndexBuffer>& ib)
{
	va->bind();
	ib->bind();
	glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr);
}

/**
*	Instanced draw call to render the same object for an specified amount. 
*
*   @param va - The VertexArray gets passed and bound
*   @param ib - The IndexBuffer gets passed and bound
*/
void Renderer::drawInstanced(std::shared_ptr<VertexArray>& va, std::shared_ptr<IndexBuffer>& ib, int numInstanced)
{
	va->bind();
	ib->bind();
	glDrawElementsInstanced(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr, numInstanced);
}

/**
*   Enable GL DEPTH TEST
*
*/
void Renderer::enableDepth()
{
	glEnable(GL_DEPTH_TEST);
}

/**
*   Disable GL DEPTH TEST
*
*/
void Renderer::disableDepth()
{
	glDisable(GL_DEPTH_TEST);
}

/**
*   Clear Dbuffer bit
*
*/
void Renderer::clearBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

/**
*   Clear the screen for re-drawing
*
*   @param f0 - RED
*   @param f1 - GREEN
*   @param f2 - BLUE
*   @param f3 - Alpha transparency.
*/
void Renderer::clear(float f0, float f1, float f2, float f3) const
{
	glClearColor(f0, f1, f2, f3);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
