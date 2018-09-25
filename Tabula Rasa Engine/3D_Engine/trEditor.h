#pragma once
#include "trModule.h"
#include "trDefs.h"

#include "SDL/include/SDL.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"

class trEditor : public trModule
{
public:

	trEditor();
	~trEditor();

	bool Init();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void Draw();
	bool UsingMouse() const;
	bool UsingKeyboard() const;


private:

	bool capture_mouse = false;
	bool capture_keyboard = false;

};