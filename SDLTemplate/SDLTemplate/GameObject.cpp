#include "GameObject.h"
#include "Scene.h"

GameObject::~GameObject()
{
	Scene::getActiveScene()->removeGameObject(this);
}

void GameObject::setScene(Scene* scene)
{
	parentScene = scene;
}

Scene* GameObject::getScene()
{
	return parentScene;
}

void GameObject::start()
{
}

void GameObject::update()
{
}

void GameObject::draw()
{
}

int GameObject::getX()
{
	return x;
}

int GameObject::getY()
{
	return y;
}
int GameObject::getWidth()
{
	return width;
}

int GameObject::getHeight()
{
	return height;
}



