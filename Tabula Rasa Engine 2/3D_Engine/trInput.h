#ifndef __trINPUT_H__
#define __trINPUT_H__

#include "trModule.h"
#include "SDL/include/SDL.h"

#include "SDL/include/SDL_gamecontroller.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;

enum trEventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum trKeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};


class trInput : public trModule
{

public:

	trInput();

	// Destructor
	virtual ~trInput();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(trEventWindow ev);

	// Check key states (includes mouse and joy buttons)
	trKeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	trKeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

private:
	bool		windowEvents[WE_COUNT];
	trKeyState*	keyboard = nullptr;
	trKeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x = 0;
	int			mouse_motion_y = 0;
	int			mouse_x = 0;
	int			mouse_y = 0;

	SDL_Joystick *joystick;

};

#endif // __trINPUT_H__