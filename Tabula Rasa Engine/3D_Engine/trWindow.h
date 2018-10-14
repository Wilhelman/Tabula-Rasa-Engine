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

	bool Awake(JSON_Object* config = nullptr);
	bool CleanUp();

	void SetTitle(const char*);

	//Setters/Getters
	void SetBrightness(float);
	void SetWidth(uint);
	void SetHeight(uint);
	void SetScale(uint);
	void SetFullscreen(bool);
	void SetResizable(bool);
	void SetBorderless(bool);
	void SetFullscreenWindowed(bool);

	uint GetWidth() const;
	uint GetHeight() const;
	uint GetScale() const;
	float GetBrightness() const;
	uint GetMonitorRefreshRate() const;
	SDL_Window* GetWindow() const;

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	uint width = 500;
	uint height = 500;
	uint scale = 1;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;
};

#endif // __ModuleWindow_H__