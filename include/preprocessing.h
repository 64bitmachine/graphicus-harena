#ifndef __PREPROCESSING_H
#define __PREPROCESSING_H

#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<contextmanager.h>

namespace cg
{
	/**
	 * prints general informations 
	 */
	void printGraphicsDetails()
	{
		const GLubyte *renderer = glGetString(GL_RENDERER);
		const GLubyte *version = glGetString(GL_VERSION);
		GLint nrVertexAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrVertexAttributes);
		std::cout << "Maximum number of vertex attributes are supported -> " << nrVertexAttributes << "\n";
		std::cout << "renderer -> " << renderer << "\n";
		std::cout << "version -> " << version << "\n";
	}

	/**
	 * GLEW manages function pointer for OpenGL so we want to initialize
	 * GLEW before we call any OpenGL functions 
	 */
	void initGLEW()
	{
		glewExperimental = GL_TRUE;
		if(glewInit() != GLEW_OK)
		{
			std::cout << "failed to initialise glew\n";
			exit(-1);
		}
	}

	void initCallBacks(ContextManager* cm)
	{
		GLFWwindow* window = cm->getContext();
	}
}

#endif