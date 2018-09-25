#ifndef __PANEL_H__
#define __PANEL_H__

#include "trDefs.h"
#include <string>
#include "SDL/include/SDL_scancode.h"

class Panel
{
public:
	Panel(const char* name, SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN);
	virtual ~Panel();

	void SwitchActive();
	bool IsActive() const;

	SDL_Scancode GetShortCut() const;

	virtual void Draw() = 0;

public:
	bool active = true;
	int width, height, posx, posy = 0;

protected:
	std::string name;
	SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN;
};

#endif // __PANEL_H__