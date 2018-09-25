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

	void TurnActive();

	bool IsActive() const;

	virtual void Draw() = 0;

	SDL_Scancode GetCurrentShortCut() const;

public:
	bool active = false;

	int x_pos, y_pos, width, height;

protected:

	std::string name;
	SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN;
};

#endif // __PANEL_H__