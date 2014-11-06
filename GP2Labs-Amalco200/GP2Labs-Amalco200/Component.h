#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

#endif

class Component
{
public:
	//no para constructor
	Component();

	//call it virtual so that runtime can call child components deconstructor
	virtual ~Component();

	//public virtual functions
	virtual void init();
	virtual void update();
	virtual void render();
	virtual void destroy();

	//public non virtual functions
	const std::string& getType();
	void toggleActive();
	bool isActive();

protected:

	std::string m_Type;
	bool m_Active;

private:
};