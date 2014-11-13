#include "Transform.h"

//constructor
Transform::Transform()
{
	m_Type = "Transform";

	m_Active = true;
}

//deconstructor
Transform::~Transform()
{
	//unsure what to enter here
}

void Transform::update()
{
	if (m_Active)
	{
		mat4 translate = glm::translate(translate, m_Position);
		mat4 scale = glm::scale(scale, m_Scale);

		mat4 rotationX = glm::rotate(rotationX, m_Rotation.x, vec3(1.0f, 0.0f, 0.0f));
		mat4 rotationY = glm::rotate(rotationY, m_Rotation.y, vec3(0.0f, 1.0f, 0.0f));
		mat4 rotationZ = glm::rotate(rotationZ, m_Rotation.z, vec3(0.0f, 0.0f, 1.0f));
		mat4 rotation = rotationX * rotationY * rotationZ;

		m_Model = translate * rotation * scale;
	}
}


//get functions

vec3 Transform::getPosition()
{
	return m_Position;
}

vec3 Transform::getScale()
{
	return m_Scale;
}

vec3 Transform::getRotation()
{
	return m_Rotation;
}

mat4 Transform::getModel()
{
	return m_Model;
}

//set functions

void Transform::setPosition(float x, float y, float z)
{
	m_Position = vec3(x, y, z);
}

void Transform::setRotation(float x, float y, float z)
{
	m_Rotation = vec3(x, y, z);
}

void Transform::setScale(float x, float y, float z)
{
	m_Scale = vec3 (x, y, z);
}