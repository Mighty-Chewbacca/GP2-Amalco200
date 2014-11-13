#ifndef MESH_H
#define MESH_H

#include "Component.h"

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

#include "Vertex.h"

class Mesh :public Component
{
public:
	Mesh();
	~Mesh();

	void init();
	void destroy();
	void copyVertexData(int count, int stride, void** data);
	void copyIndexData(int count, int stride, void** data);
	void bind();

	int getIndexCount();
	int getVertexCount();

protected:

private:
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_VAO;

	int m_VertexCount;
	int m_IndexCount;


};

#endif