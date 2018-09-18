#ifndef __trAUDIO_H__
#define __trAUDIO_H__

#include "SDL_mixer\include\SDL_mixer.h"
#include <vector>
#include "trApp.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class trAudio : public ctModule
{
public:

	trAudio();

	// Destructor
	virtual ~trAudio();

	// Called before render is available
	bool Awake(pugi::xml_node&);


	bool Update(float dt);


	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	//Pause a playing music
	bool PauseMusic(float fade_time = DEFAULT_MUSIC_FADE_TIME);

	//Stop a FX
	bool HaltFX(int id = -1, int fadeseconds = 0);

	//Pause a FX
	bool PauseFX(int id = -1);

	//Resume a previously paused FX
	bool ResumeFX(int id = -1);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	//Return the channel used or -1 if errors happened
	int PlayFx(unsigned int fx, int repeat = 0, int channel = -1);

	//Set Volumes
	void SetMusicVolume(uint percent);
	void SetFXVolume(uint percent);

	//InitAudio

	void InitAudio();

public:
	//volumes
	uint music_volume_percent = 10;
	uint fx_volume_percent = 10;

private:

	_Mix_Music*			music;
	std::vector <Mix_Chunk*> fx;

public:
	bool device_connected = false;

};

#endif // __trAUDIO_H__
