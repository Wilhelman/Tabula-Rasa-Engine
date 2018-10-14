#ifndef __trWindow_H__
#define __trWindow_H__

#include "trModule.h"

class trApp;
class SDL_Window;
class SDL_Surface;

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
	void SetWidth(uint width);
	void SetHeight(uint height);
	void SetScale(uint scale);
	void SetFullscreen(bool fullscreen);
	void SetResizable(bool resizable);
	void SetBorderless(bool borderless);
	void SetFullscreenWindowed(bool fullscreen_windowed);

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

	// Config settings
	uint width = 500u;
	uint height = 500u;
	uint scale = 1u;
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;
};

#endif // __ModuleWindow_H__