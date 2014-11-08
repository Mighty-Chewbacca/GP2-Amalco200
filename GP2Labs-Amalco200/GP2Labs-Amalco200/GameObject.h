#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>

class Component;

#endif

class GameObject
{
public:
	void init();
	void update();
	void render();
	void destroy();

	const std::string& getname();
	void setName(const std::string& name);
	void addComponent(Component * component);

protected:

private:

	std::string m_name;
	std::vector<Component*> m_Components;
};