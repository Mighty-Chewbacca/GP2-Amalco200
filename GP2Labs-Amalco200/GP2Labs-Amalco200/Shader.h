#ifndef Shader_h
#define Shader_h

//includes
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <iostream>
#include <fstream>
#include <string>

#endif

GLuint loadShaderFromMemory(const char * pMem, SHADER_TYPE shaderType);

GLuint loadShaderFromFile(const std::string& filename, SHADER_TYPE shaderType);

enum SHADER_TYPE
{
	VERTEX_SHADER = GL_VERTEX_SHADER,
	FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

bool checkForCompilerErrors(GLuint shaderProgram);

bool checkForLinkErrors(GLuint program);