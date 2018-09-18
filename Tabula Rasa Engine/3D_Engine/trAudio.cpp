#include "trDefs.h"
#include "trLog.h"
#include "trAudio.h"
#include <list>
#include "trApp.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ctAudio::trAudio() : trModule()
{
	music = NULL;
	name = "audio";
}

// Destructor
ctAudio::~trAudio()
{}

// Called before render is available
bool trAudio::Awake(pugi::xml_node& audioNode)
{
	LOG("Loading Audio Mixer");

	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	if (SDL_GetNumAudioDevices(0) > 0)
		device_connected = true;


	if (device_connected)
	{
		InitAudio();
	}

	return true;
}


void trAudio::InitAudio()
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	Mix_AllocateChannels(16);
	
}


bool trAudio::Update(float dt)
{
	if (device_connected == false && SDL_GetNumAudioDevices(0) > 0)
	{
		device_connected = true;
		InitAudio();
	}

	return true;
}



// Called before quitting
bool trAudio::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	for (uint i = 0; i < fx.size(); ++i)
		Mix_FreeChunk(fx[i]);
	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool trAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if (!device_connected)
		return false;

	if (music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

bool trAudio::PauseMusic(float fade_time)
{
	if (music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}
	}
	return Mix_PlayingMusic() == 0;
}

bool trAudio::HaltFX(int id, int fadeseconds)
{
	if (!device_connected)
		return false;

	if (id == -1)
	{
		Mix_HaltChannel(-1);
	}
	else
		for (int i = 0; i < 16; ++i)
		{
			if (Mix_GetChunk(i) == fx[id - 1])
				Mix_HaltChannel(i);
		}

	return true;
}

bool trAudio::PauseFX(int id)
{
	if (!device_connected)
		return false;

	if (id == -1)
		Mix_Pause(-1);
	else
		for (int i = 0; i < 16; ++i)
		{
			if (Mix_GetChunk(i) == fx[id - 1])
				Mix_Pause(i);
			break;
		}
	return true;
}

bool trAudio::ResumeFX(int id)
{
	if (!device_connected)
		return false;

	if (id == -1)
	{
		Mix_Resume(-1);
	}
	else
	{
		for (int i = 0; i < 16; ++i)
		{
			if (Mix_GetChunk(i) == fx[id - 1])
				Mix_Resume(i);
			break;
		}
	}
}

// Load WAV
unsigned int trAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if (!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path, 1);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play a previously loaded WAV
//Return the channel used or -1 if errors happened
int trAudio::PlayFx(unsigned int id, int repeat, int channel)
{
	if (!device_connected)
		return -1;

	if (id > 0 && id <= fx.size())
	{
		return Mix_PlayChannel(channel, fx[id - 1], repeat);
	}

	return -1;
}

void trAudio::SetMusicVolume(uint percent)
{
	if (!device_connected)
		return;

	MusicVolumePercent = percent;
	Mix_VolumeMusic((MIX_MAX_VOLUME * MusicVolumePercent) / 100);
}

void trAudio::SetFXVolume(uint percent)
{
	if (!device_connected)
		return;

	FXVolumePercent = percent;
	Mix_Volume(-1, (MIX_MAX_VOLUME * FXVolumePercent) / 100);
}