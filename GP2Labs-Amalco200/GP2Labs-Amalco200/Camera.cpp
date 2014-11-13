#include "Camera.h"
#include "GameObject.h"
#include"Transform.h"

Camera::Camera()
{
	m_Type = "Camera";
	m_LookAt = vec3(0.0f, 0.0f, 0.0f);
	m_Up = vec3(0.0f, 1.0f, 0.0f);
	m_NearClip = 0.1f;
	m_FarClip = 100.0f;
	m_FOV = 45.0f;
	m_AspectRatio = 16.0f / 9.0f;
	m_ViewMatrix = mat4();
	m_ProjectionMatrix = mat4();
}

Camera::~Camera()
{

}

void Camera::setLookAt(float x, float y, float z)
{
	m_LookAt = vec3(x, y, z);
}

void Camera::setUp(float x, float y, float z)
{
	m_Up = vec3(x, y, z);
}


void Camera::setNearClip(float nearClip)
{
	m_NearClip = nearClip;
}

void Camera::setFarClip(float farClip)
{
	m_FarClip = farClip;
}

void Camera::setFOV(float FOV)
{
	m_FOV = FOV;
}

void Camera::setAspectRatio(float ratio)
{
	m_AspectRatio = ratio;
}

mat4& Camera::getViewMatrix()
{
	return m_ViewMatrix;
}

mat4& Camera::getProjectionMatrix()
{
	return m_ProjectionMatrix;
}

void Camera::update()
{
	m_Position = m_Parent->getTransform()->getPosition();
	m_ProjectionMatrix = glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
	m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, m_Up);
}