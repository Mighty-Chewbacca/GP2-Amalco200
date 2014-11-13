#include "Component.h"
#include "GameObject.h"


//constructor
Component::Component()
{
	m_Type = "Component";
	m_Parent = NULL;
	m_Active = true;
}

//deconstructor
Component::~Component()
{

}

void Component::init()
{

}

void Component::update()
{

}

void Component::render()
{

}

void Component::destroy()
{

}

const std::string& Component::getType()
{
	return m_Type;
}

bool Component::isActive()
{
	return m_Active;
}

void Component::setParent(GameObject *parent)
{
	m_Parent = parent;
}

GameObject * Component::getParent()
{
	return m_Parent;
}

void Component::toggleActive()
{
	m_Active = !m_Active;
}