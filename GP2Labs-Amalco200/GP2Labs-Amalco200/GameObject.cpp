//includes
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

GameObject::GameObject()
{
	m_Transform = NULL;
	m_Mesh = NULL;
	m_Material = NULL;
	m_Camera = NULL;
	m_name = "GameObject";
	m_Parent = NULL;
}

GameObject::~GameObject()
{

}

void GameObject::init()
{
	for (auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
	{
		(*iter)->init();
	}

	//call init on children
	for (auto iter = m_Children.begin(); iter != m_Children.end(); iter++)
	{
		(*iter)->init();
	}
}

void GameObject::update()
{
	for (auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
	{
		(*iter)->update();
	}

	//call update on children
	for (auto iter = m_Children.begin(); iter != m_Children.end(); iter++)
	{
		(*iter)->update();
	}
}

void GameObject::render()
{
	for (auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
	{
		(*iter)->render();
	}

	//call render on children as well
	for (auto iter = m_Children.begin(); iter != m_Children.end(); iter++)
	{
		(*iter)->render();
	}
}

void GameObject::destroy()
{
	auto iter = m_Components.begin();
	while (iter != m_Components.end())
	{
		(*iter)->destroy();
		if ((*iter))
		{
			delete (*iter);
			(*iter) = NULL;
			iter = m_Components.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	m_Components.clear();

	//iter through and destroy children as well
	auto iterChildren = m_Children.begin();
	while (iterChildren != m_Children.end())
	{
		(*iterChildren)->destroy();
		if ((*iterChildren))
		{
			delete (*iterChildren);
			(*iterChildren) = NULL;
			iterChildren = m_Children.erase(iterChildren);
		}
		else
		{
			iterChildren++;
		}
	}
	m_Children.clear();
}

void GameObject::addComponent(Component * component)
{
	component->setParent(this);
	m_Components.push_back(component);
}

const std::string& GameObject::getname()
{
	return m_name;
}

void GameObject::setName(const std::string& name)
{
	m_name = name;
}

void GameObject::setTransform(Transform *transform)
{
	m_Transform = transform;
	addComponent(transform);
}

void GameObject::setMesh(Mesh *mesh)
{
	m_Mesh = mesh;
	addComponent(mesh);
}

void GameObject::setMaterial(Material *material)
{
	m_Material = material;
	addComponent(material);
}

void GameObject::setCamera(Camera *camera)
{
	m_Camera = camera;
	addComponent(camera);
}

void GameObject::setParent(GameObject *parent)
{
	m_Parent = parent;
}

Transform * GameObject::getTransform()
{
	return m_Transform;
}

Mesh * GameObject::getMesh()
{
	return m_Mesh;
}

Material * GameObject::getMaterial()
{
	return m_Material;
}

Camera * GameObject::getCamera()
{
	return m_Camera;
}

GameObject * GameObject::getParent()
{
	return m_Parent;
}

void GameObject::addChild(GameObject* child)
{
	m_Children.push_back(child);
}

int GameObject::getChildCount()
{
	return m_Children.size();
}

GameObject* GameObject::getChild(int index)
{
	if (index <= m_Children.size())
	{
		return m_Children[index];
	}

	//do some error message here
}
