#include "SoundManager.h"

SoundManager::SoundManager()
{
	//music = NULL;
}

SoundManager::~SoundManager()
{
	
}

SoundManager* SoundManager::getInstance()
{
	static SoundManager *soundManager = new SoundManager();
	return soundManager;
}

Mix_Chunk* SoundManager::loadSound(const char* filename)
{
	Mix_Chunk* sound = getInstance()->getLoadedSound(filename);
	if (sound == NULL)
	{
		sound = Mix_LoadWAV(filename);

		SoundClip clip;
		clip.fileName = filename;
		clip.sound = sound;
		getInstance()->loadedSounds.push_back(clip);
	}
	return sound;
}

void SoundManager::playSound(const char* filename, int channel)
{
	Mix_Chunk* sound = getInstance()->getLoadedSound(filename);
	if (sound == NULL) loadSound(filename);

	sound = getInstance()->getLoadedSound(filename);
	Mix_PlayChannel(channel, sound, 0);
}

void SoundManager::playSound(Mix_Chunk* sound, int channel)
{
	if (sound == NULL)
	{
		std::cout << "Warning: Attempting to play a sound with a null pointer" << std::endl;
		return;
	}
	Mix_PlayChannel(channel, sound, 0);
}

void SoundManager::loadMusic(const char* filename)
{
	SoundManager* sm = getInstance();

	if (sm->musicChunk != NULL)
	{
		Mix_HaltChannel(sm->musicChannel);
		Mix_FreeChunk(sm->musicChunk);
		sm->musicChunk = NULL;
		sm->musicChannel = -1;
	}

	sm->musicChunk = Mix_LoadWAV(filename);

	if (sm->musicChunk == NULL)
	{
		std::cout << "Failed to load music: " << Mix_GetError() << std::endl;
	}
}

void SoundManager::playMusic(int loop)
{
	SoundManager* sm = getInstance();

	if (sm->musicChunk == NULL)
	{
		std::cout << "Music is not loaded!" << std::endl;
		return;
	}
	sm->musicChannel = Mix_PlayChannel(-1, sm->musicChunk, loop ? -1 : 0);
}

void SoundManager::stopMusic()
{
	SoundManager* sm = getInstance();

	if (sm->musicChannel != -1)
	{
		Mix_HaltChannel(sm->musicChannel);
		sm->musicChannel = -1;
	}
}

void SoundManager::setMusicVolume(int volume)
{
	SoundManager* sm = getInstance();

	if (sm->musicChunk != NULL)
	{
		sm->musicChunk->volume = volume;
	}
}

Mix_Chunk* SoundManager::getLoadedSound(const char* filename)
{
	for (int i = 0; i < loadedSounds.size(); i++)
	{
		if (loadedSounds[i].fileName == filename)
			return loadedSounds[i].sound;
	}

	return NULL;
}

