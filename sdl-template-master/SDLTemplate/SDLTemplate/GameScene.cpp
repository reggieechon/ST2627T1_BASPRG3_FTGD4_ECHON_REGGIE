#include "GameScene.h"
#include "player.h"
#include "platform.h"

GameScene::GameScene()
{
	// Register and add game objects on constructor
}

GameScene::~GameScene()
{
	addGameObject(new player());
	addGameObject(new platform(250, 540, 300, 29));
	addGameObject(new platform(450, 600, 250, 29));
	addGameObject(new platform(100, 200, 250, 29));
}

void GameScene::start()
{
	Scene::start();
	// Initialize any scene logic here
}

void GameScene::draw()
{
	Scene::draw();
}

void GameScene::update()
{
	Scene::update();
}
