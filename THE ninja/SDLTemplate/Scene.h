#pragma once
#include <vector>
#include "app.h"
#include <algorithm>

extern App app;
class GameObject;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	static void setActiveScene(Scene* scene);
	static Scene* getActiveScene();

	void addGameObject(GameObject* obj);
	void removeGameObject(GameObject* obj);

	std::vector <GameObject*>& getObjects() { return objects; }

	virtual void start();
	virtual void update();
	virtual void draw();

private:
	std::vector<GameObject*> objects;
	bool hasStarted;
	static Scene* activeScene;
};

