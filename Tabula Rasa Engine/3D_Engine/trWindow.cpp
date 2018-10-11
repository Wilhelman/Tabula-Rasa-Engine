#include "trDefs.h"
#include "trApp.h"
#include "trWindow.h"

trWindow::trWindow() : trModule()
{
	name = "window";
	window = NULL;
	screen_surface = NULL;
}

// Destructor
trWindow::~trWindow()
{
}

// Called before render is available
bool trWindow::Awake(JSON_Object* config)
{
	TR_LOG("trWindow: Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		TR_LOG("trWindow: SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		if (config != nullptr) {
			this->SetWidth(json_object_get_number(config, "width"));
			this->SetHeight(json_object_get_number(config, "height"));
			this->SetScale(json_object_get_number(config, "scale"));
			this->SetFullscreen(json_object_get_boolean(config, "fullscreen"));
			this->SetResizable(json_object_get_boolean(config, "resizable"));
			this->SetBorderless(json_object_get_boolean(config, "borderless"));
			this->SetFullscreenWindowed(json_object_get_boolean(config, "fullscreen_window"));
		}
		
		//Create window
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if (fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (borderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (fullscreen_desktop)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(App->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			TR_LOG("trWindow: Window could not be created! SDL_Error: %s\n", SDL_GetError());
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
	TR_LOG("trWindow: CleanUp");

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
	App->SetTitle(title);
}

void trWindow::SetBrightness(float set)
{
	CAP(set);
	if (SDL_SetWindowBrightness(window, set) != 0)
		TR_LOG("trWindow: Could not change window brightness: %s\n", SDL_GetError());
}

float trWindow::GetBrightness() const
{
	return SDL_GetWindowBrightness(window);
}

void trWindow::SetWidth(uint width)
{
	SDL_SetWindowSize(window, width, height);
	this->width = width;
}

void trWindow::SetHeight(uint height)
{
	SDL_SetWindowSize(window, width, height);
	this->height = height;
}

void trWindow::SetScale(uint scale)
{
	this->scale = scale;
}

uint trWindow::GetMonitorRefreshRate() const
{
	uint ret = 0;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		TR_LOG("trWindow: SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
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
				TR_LOG("trWindow: Could not switch to fullscreen: %s\n", SDL_GetError());
			fullscreen_desktop = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				TR_LOG("trWindow: Could not switch to windowed: %s\n", SDL_GetError());
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
	//if (set != borderless && fullscreen == false && fullscreen_desktop == false)
	//{
		borderless = set;
		SDL_SetWindowBordered(window, (SDL_bool)!borderless);
	//}
}

void trWindow::SetFullscreenWindowed(bool set)
{
	if (set != fullscreen_desktop)
	{
		fullscreen_desktop = set;
		if (fullscreen_desktop == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				TR_LOG("trWindow: Could not switch to fullscreen desktop: %s\n", SDL_GetError());
			fullscreen = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				TR_LOG("trWindow:Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}

uint trWindow::GetWidth() const
{
	return width * scale;
}

uint trWindow::GetHeight() const
{
	return height * scale;
}

uint trWindow::GetScale() const
{
	return scale;
}
