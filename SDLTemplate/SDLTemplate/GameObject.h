#pragma once
class Scene;
class GameObject
{
public:
	~GameObject();
	// Sets the scene pointer for the gameobject
	void setScene(Scene* scene);
	// Returns the scene pointer where this game object is found in
	Scene* getScene();

	virtual void start();
	virtual void update();
	virtual void draw();

	int getX();
	int getY();
	int getWidth();
	int getHeight();

protected:
	int x; 
	int y;
	int width;
	int height;

private:
	// Scene where this object belongs to
	Scene* parentScene;
};

