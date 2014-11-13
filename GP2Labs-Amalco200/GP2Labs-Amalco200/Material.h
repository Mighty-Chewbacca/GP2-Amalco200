#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <string>
#include <fstream>
#include "Component.h"

#include "Vertex.h"

//header file for glew
#include <GL/glew.h>

//header for sdl2 functionality
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>

//maths headers
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <gl\GLU.h>
#include <SDL_opengl.h>

class Material :public Component
{
public:

	Material();
	~Material();

	void destroy();
	bool loadShader(const std::string& vsFilename, const std::string& fsFilename);
	void bind();
	GLint getUniformLocation(const std::string& name);


protected:

private:

	GLuint m_ShaderProgram;

};

#endif