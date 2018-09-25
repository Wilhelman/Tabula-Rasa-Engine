
#include "trApp.h"
#include "trWindow.h"
#include "trEditor.h"
#include "trRenderer3D.h"

#include "PanelAbout.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include "SDL\include\SDL.h"
#include "SDL\include\SDL_opengl.h"

trEditor::trEditor() : trModule()
{

}

// Destructor
trEditor::~trEditor()
{
}

// Called before render is available
bool trEditor::Init()
{
	TR_LOG("Init editor gui with imgui lib version %s", ImGui::GetVersion());

	ImGui_ImplSDL2_InitForOpenGL(App->win->window, App->render->context);
	ImGui_ImplOpenGL2_Init();

	ImGuiIO& io = ImGui::GetIO();

	//Panels

	panels.push_back(about = new PanelAbout());

	return true;
}

bool trEditor::PreUpdate(float dt)
{

	//todo: do a getter for the window
	ImGui_ImplSDL2_NewFrame(App->win->window);

	ImGuiIO& io = ImGui::GetIO();
	capture_keyboard = io.WantCaptureKeyboard;
	capture_mouse = io.WantCaptureMouse;

	return true;
}

bool trEditor::Update(float dt)
{

	return true;
}

bool trEditor::PostUpdate(float dt)
{

	return true;
}

// Called before quitting
bool trEditor::CleanUp()
{
	TR_LOG("Cleaning trEditor");

	return true;
}