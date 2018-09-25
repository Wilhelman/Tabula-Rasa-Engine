#include "trDefs.h"
#include "trApp.h"
#include "trWindow.h"

trWindow::trWindow() : trModule()
{
	name = "Window";
	window = NULL;
	screen_surface = NULL;
}

// Destructor
trWindow::~trWindow()
{
}

// Called before render is available
bool trWindow::Awake(pugi::xml_node&)
{
	TR_LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		TR_LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = SCREEN_WIDTH * SCREEN_SIZE;
		height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if (WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
			fullscreen = true;
		}

		if (WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
			resizable = true;
		}

		if (WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
			borderless = true;
		}

		if (WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			fullscreen_desktop = true;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			TR_LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool trWindow::CleanUp()
{
	TR_LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void trWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void trWindow::SetBrightness(float set)
{
	CAP(set);
	if (SDL_SetWindowBrightness(window, set) != 0)
		TR_LOG("Could not change window brightness: %s\n", SDL_GetError());
}

float trWindow::GetBrightness() const
{
	return SDL_GetWindowBrightness(window);
}

void trWindow::GetMaxMinSize(uint & min_width, uint & min_height, uint & max_width, uint & max_height) const
{
	min_width = 640;
	min_height = 480;
	max_width = 3000;
	max_height = 2000;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		TR_LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	else
	{
		max_width = dm.w;
		max_height = dm.h;
	}
}

void trWindow::SetWidth(uint width)
{
	SDL_SetWindowSize(window, width, height);
	this->width = width;
}

void trWindow::SetHeigth(uint height)
{
	SDL_SetWindowSize(window, width, height);
	this->height = height;
}

uint trWindow::GetRefreshRate() const
{
	uint ret = 0;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		TR_LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	else
		ret = dm.refresh_rate;

	return ret;
}

void trWindow::SetFullscreen(bool set)
{
	if (set != fullscreen)
	{
		fullscreen = set;
		if (fullscreen == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
				TR_LOG("Could not switch to fullscreen: %s\n", SDL_GetError());
			fullscreen_desktop = false;
			SDL_Log("this is a test");
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				TR_LOG("Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}

void trWindow::SetResizable(bool set)
{
	// cannot be changed while the program is running, but we can save the change
	resizable = set;
}

void trWindow::SetBorderless(bool set)
{
	if (set != borderless && fullscreen == false && fullscreen_desktop == false)
	{
		borderless = set;
		SDL_SetWindowBordered(window, (SDL_bool)!borderless);
	}
}

void trWindow::SetFullScreenDesktop(bool set)
{
	if (set != fullscreen_desktop)
	{
		fullscreen_desktop = set;
		if (fullscreen_desktop == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				TR_LOG("Could not switch to fullscreen desktop: %s\n", SDL_GetError());
			fullscreen = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				TR_LOG("Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}