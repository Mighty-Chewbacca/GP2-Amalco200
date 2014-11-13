#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>

class Component;
class Transform;
class Mesh;
class Material;
class Camera;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void init();
	void update();
	void render();
	void destroy();

	const std::string& getname();
	void setName(const std::string& name);
	void addComponent(Component * component);

	void setTransform(Transform *transform);
	void setMesh(Mesh *mesh);
	void setMaterial(Material *material);
	void setCamera(Camera *camera);

	Transform * getTransform();
	Mesh * getMesh();
	Material * getMaterial();
	Camera * getCamera();

protected:

private:

	std::string m_name;
	std::vector<Component*> m_Components;

	Transform *m_Transform;
	Mesh *m_Mesh;
	Material *m_Material;
	Camera *m_Camera;
	GameObject *m_Parent;
};


#endif