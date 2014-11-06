#include <string>

class Component
{
public:
	//no para constructor
	Component();

	//call it virtual so that runtime can call child components deconstructor
	virtual ~Component();

	//public virtual functions
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

	//public non virtual functions
	const std::string& GetType();
	void ToggleActive();
	bool IsActive();

protected:

	std::string m_Type;
	bool m_Active;

private:
};