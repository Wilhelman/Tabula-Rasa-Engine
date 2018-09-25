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

	bool Awake(pugi::xml_node&);
	bool CleanUp();

	void SetTitle(const char* title);

	//Setters/Getters
	float GetBrightness() const;
	void SetBrightness(float set);
	void GetMaxMinSize(uint& min_width, uint& min_height, uint& max_width, uint& max_height) const;
	void SetWidth(uint width);
	void SetHeigth(uint height);
	uint GetRefreshRate() const;
	void SetFullscreen(bool set);
	void SetResizable(bool set);
	void SetBorderless(bool set);
	void SetFullScreenDesktop(bool set);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	//this attributes should have getters and setters..
	int width = 0;
	int height = 0;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;
	//end this attributes warning
};

#endif // __ModuleWindow_H__