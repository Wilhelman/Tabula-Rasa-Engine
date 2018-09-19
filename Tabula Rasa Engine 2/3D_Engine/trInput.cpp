#include "trDefs.h"
#include "trLog.h"
#include "trApp.h"
#include "trInput.h"
#include "trWindow.h"
#include "trRenderer3D.h"

#define MAX_KEYS 300

trInput::trInput() : trModule()
{
	name ="input";

	keyboard = new trKeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(trKeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(trKeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
trInput::~trInput()
{
	delete[] keyboard;
}

// Called before render is available
bool trInput::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool trInput::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool trInput::PreUpdate(float dt)
{
	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;

		case SDL_MOUSEMOTION:
			mouse_motion_x = event.motion.xrel / SCREEN_SIZE;
			mouse_motion_y = event.motion.yrel / SCREEN_SIZE;
			mouse_x = event.motion.x / SCREEN_SIZE;
			mouse_y = event.motion.y / SCREEN_SIZE;
			//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;
		}
	}

	return true;
}

// Called before quitting
bool trInput::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool trInput::GetWindowEvent(trEventWindow ev)
{
	return windowEvents[ev];
}

void trInput::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void trInput::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}