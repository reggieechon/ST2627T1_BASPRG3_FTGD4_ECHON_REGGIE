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
	static Mix_Chunk* loadMusic(const char* filename);
	static void playMusic(int loop);
	Mix_Music* music;
private:
	std::vector<SoundClip> loadedSounds;

	Mix_Chunk* getLoadedSound(const char* filename);
};