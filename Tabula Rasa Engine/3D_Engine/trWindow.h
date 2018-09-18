#ifndef __trWINDOW_H__
#define __trWINDOW_H__

#include "trModule.h"
#include <string>

struct SDL_Window;
struct SDL_Surface;

class trWindow : public trModule
{
public:

	trWindow();

	// Destructor
	virtual ~trWindow();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* new_title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint GetScale() const;
	float GetWScalade() const;
	float GetHScalade() const;

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	SDL_Surface* iconExe = nullptr;

private:
	std::string	title;
	float		w_scalade = 0u;
	float		h_scalade = 0u;
	uint		width = 0u;
	uint		height = 0u;
	uint		scale = 0u;
};

#endif // __trWINDOW_H__