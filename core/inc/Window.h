#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {

private:

	GLFWwindow* mainWindow; // Must be standard pointer because of GLFW!
	GLuint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024]; // ascii length
	
	static void inputHandler(GLFWwindow* window, int key, int code, int action, int mode); // Must be static!
	static void mouseHandler(GLFWwindow* window, double xPos, double yPos);

	void callback();

	GLfloat changeX;
	GLfloat changeY;
	GLfloat lastPosX;
	GLfloat lastPosY;

	bool initialPos;

public:

	Window();
	~Window();
	Window(GLint wWidth, GLint wHeight);

	bool* retrieveKeys() { return keys; }
	bool shouldClose() { return glfwWindowShouldClose(mainWindow); }

	GLfloat getChangeX();
	GLfloat getChangeY();

	int initialise();

	void swapBuffer() { return glfwSwapBuffers(mainWindow); }
	void closeWindow();
	void updateFPS();

	inline GLfloat getBufferWidth() { return bufferWidth; }
	inline GLfloat getBufferHeight() { return bufferHeight; }

};