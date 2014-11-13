#ifndef CAMERA_H
#define CAMERA_H

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

class Camera :public Component
{
public:

	Camera();
	~Camera();

	void setPosition(vec3 position);
	void setLookAt(float x, float y, float z);
	void setUp(float x, float y, float z);
	void setFOV(float FOV);
	void setAspectRatio(float aspectRatio);
	void setNearClip(float nearClip);
	void setFarClip(float farClip);

	mat4& getViewMatrix();
	mat4& getProjectionMatrix();

	void update();


protected:

private:

	vec3 m_Position;
	vec3 m_LookAt;
	vec3 m_Up;
	float m_FOV;
	float m_AspectRatio;
	float m_NearClip;
	float m_FarClip;
	mat4 m_ViewMatrix;
	mat4 m_ProjectionMatrix;
};


#endif