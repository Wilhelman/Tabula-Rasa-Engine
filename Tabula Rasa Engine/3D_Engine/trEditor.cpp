
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

	
	//Panels

	panels.push_back(about = new PanelAbout());

	return true;
}

bool trEditor::Start()
{
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplSDL2_InitForOpenGL(App->win->window, App->render->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsDark();

	return true;
}

bool trEditor::PreUpdate(float dt)
{

	ImGui_ImplOpenGL2_NewFrame();
	//todo: do a getter for the window
	ImGui_ImplSDL2_NewFrame(App->win->window);
	ImGui::NewFrame();

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
	ImGui::Render();
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());


	glLineWidth(1.0f);

	glBegin(GL_LINES);
	return true;
}

// Called before quitting
bool trEditor::CleanUp()
{
	TR_LOG("Cleaning trEditor");

	return true;
}