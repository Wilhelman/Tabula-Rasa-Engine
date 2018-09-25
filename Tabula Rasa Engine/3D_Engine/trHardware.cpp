#include "trHardware.h"
#include "SDL/include/SDL.h"


trHardware::trHardware()
{
	SDL_version sdl_version;
	SDL_GetVersion(&sdl_version);

	version_major = sdl_version.major;
	version_minor = sdl_version.minor;
	version_patch = sdl_version.patch;



	
}

trHardware::~trHardware()
{
}
