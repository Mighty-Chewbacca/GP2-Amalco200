#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"

//maths headers
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform :public Component
{
public:
	//constructor
	Transform();

	//deconstructor
	~Transform();

	void update();
	//get methods
	vec3 getPosition();
	vec3 getRotation();
	vec3 getScale();

	mat4 getModel();

	//set methods
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

protected:

private:
	vec3 m_Position;
	vec3 m_Scale;
	vec3 m_Rotation;
	mat4 m_Model;

};

#endif
