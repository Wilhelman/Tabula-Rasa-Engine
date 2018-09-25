#include "Panel.h"

// ---------------------------------------------------------
Panel::Panel(const char* name, SDL_Scancode shortcut) : name(name), shortcut(shortcut)
{}

// ---------------------------------------------------------
Panel::~Panel()
{}

// ---------------------------------------------------------
void Panel::TurnActive()
{
	active = !active;
}

// ---------------------------------------------------------
bool Panel::IsActive() const
{
	return active;
}

// ---------------------------------------------------------
SDL_Scancode Panel::GetCurrentShortCut() const
{
	return shortcut;
}
