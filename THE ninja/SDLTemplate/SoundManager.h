#pragma once
#include "common.h"
#include "defs.h"
#include <vector>
#include <string>

#include <SDL_mixer.h>

struct SoundClip
{
	Mix_Chunk* sound;
	const char* fileName;
};

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	static SoundManager* getInstance();
	static Mix_Chunk* loadSound(const char* filename);
	static void playSound(const char* filename, int channel = -1);
	static void playSound(Mix_Chunk* sound, int channel = -1);
	static void loadMusic(const char* filename);
	static void playMusic(int loop);
	static void stopMusic();
	static void setMusicVolume(int volume);
	Mix_Chunk* musicChunk = NULL;
	int musicChannel = 1;
private:
	std::vector<SoundClip> loadedSounds;

	Mix_Chunk* getLoadedSound(const char* filename);
};