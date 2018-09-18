#ifndef __trWindow_H__
#define __trWindow_H__

#include "trModule.h"
#include "SDL/include/SDL.h"

class trApp;

class trWindow : public trModule
{
public:

	trWindow();

	// Destructor
	virtual ~trWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
};

#endif // __ModuleWindow_H__