#include "GameScene.h"
#include "player.h"
#include "platform.h"

GameScene::GameScene()
{
	player = new Player();
	this->addGameObject(player);
	SoundManager::loadMusic("music/BackgroundMusic.mp3");
	//music->volume = 24;

	addGameObject(new platform(250, 540, 300, 29));
	addGameObject(new platform(450, 600, 250, 29));
	addGameObject(new platform(100, 200, 250, 29));
}

GameScene::~GameScene()
{
	
}

void GameScene::start()
{
	Scene::start();
	SoundManager::playMusic(1);
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
