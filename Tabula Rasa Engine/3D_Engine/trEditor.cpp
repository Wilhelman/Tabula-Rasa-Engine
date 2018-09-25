
#include "trApp.h"
#include "trWindow.h"
#include "trEditor.h"
#include "trRenderer3D.h"
#include "trInput.h"

#include "Panel.h"
#include "PanelAbout.h"
#include "PanelConfiguration.h"
#include "PanelConsole.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include "SDL\include\SDL.h"
#include "SDL\include\SDL_opengl.h"

#include <vector>

trEditor::trEditor() : trModule()
{
	this->name = "Editor";
}

// Destructor
trEditor::~trEditor()
{
}

// Called before render is available
bool trEditor::Init()
{
	TR_LOG("Init editor gui with imgui lib version %s", ImGui::GetVersion());

	return true;
}

bool trEditor::Start()
{
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(App->win->window, App->render->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsDark();

	//Panels
	panels.push_back(about = new PanelAbout());
	panels.push_back(config = new PanelConfiguration());
	panels.push_back(console = new PanelConsole());

	return true;
}

bool trEditor::PreUpdate(float dt)
{

	ImGuiIO& io = ImGui::GetIO();
	user_using_keyboard = io.WantCaptureKeyboard;
	user_using_mouse = io.WantCaptureMouse;

	return true;
}

bool trEditor::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	//todo: do a getter for the window
	ImGui_ImplSDL2_NewFrame(App->win->window);
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "Alt+F4"))
				return false;

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console", "1"))
				console->TurnActive();

			if (ImGui::MenuItem("Configuration", "4"))
				config->TurnActive();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Gui Demo"))
				show_demo_window = true;

			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://github.com/Wilhelman/Tabula-Rasa-Engine/");

			if (ImGui::MenuItem("Download latest"))
				App->RequestBrowser("https://github.com/Wilhelman/Tabula-Rasa-Engine/releases");
			
			if (ImGui::MenuItem("Report a bug"))
				App->RequestBrowser("https://github.com/Wilhelman/Tabula-Rasa-Engine/issues");

			if (ImGui::MenuItem("About"))
				about->TurnActive();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	// Draw active panels
	std::vector<Panel*>::iterator it = panels.begin();
	while( it != panels.end())
	{
		Panel* panel = (*it);

		if (App->input->GetKey(panel->GetCurrentShortCut()) == KEY_DOWN)
			panel->TurnActive();

		if (panel->IsActive())
			panel->Draw();

		it++;
	}

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	return true;
}

bool trEditor::PostUpdate(float dt)
{
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glLineWidth(1.0f);

	glBegin(GL_LINES);

	glEnd();

	return true;
}

// Called before quitting
bool trEditor::CleanUp()
{
	TR_LOG("Cleaning trEditor");
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(App->render->context);
	SDL_DestroyWindow(App->win->window);
	SDL_Quit();

	return true;
}

void trEditor::InfoFPSMS(float current_fps, float current_ms)
{
	(config != nullptr) ? config->FillChartFpsInfo(current_fps, current_ms) : TR_LOG("trEditor error: configuration == nullptr");
}
