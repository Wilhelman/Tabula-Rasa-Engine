
#include "trApp.h"
#include "trWindow.h"
#include "trEditor.h"
#include "trRenderer3D.h"
#include "trInput.h"

#include "Panel.h"
#include "PanelAbout.h"
#include "PanelConfiguration.h"
#include "PanelConsole.h"
#include "PanelInspector.h"

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

bool trEditor::Start()
{
	// Setup Dear ImGui binding
	TR_LOG("trEditor: Init editor with IMGUI lib version %s", ImGui::GetVersion());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->render->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsDark();

	//Panels
	panels.push_back(about = new PanelAbout());
	panels.push_back(config = new PanelConfiguration());
	panels.push_back(console = new PanelConsole(init_logs));
	panels.push_back(inspector = new PanelInspector());

	return true;
}

bool trEditor::PreUpdate(float dt)
{

	ImGuiIO& io = ImGui::GetIO();
	user_using_keyboard = io.WantCaptureKeyboard;
	user_using_mouse = io.WantCaptureMouse;

	if (have_to_quit)
		return false;

	return true;
}

bool trEditor::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	//todo: do a getter for the window
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
				App->Save();
			if (ImGui::MenuItem("Load"))
				App->Load();
			if (ImGui::MenuItem("Quit", "Alt+F4"))
				have_to_quit = true;

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console", "1"))
				console->TurnActive();

			if (ImGui::MenuItem("Configuration", "2"))
				config->TurnActive();

			if (ImGui::MenuItem("Inspector", "I"))
				inspector->TurnActive();

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
	return true;
}

// Called before quitting
bool trEditor::CleanUp()
{
	Log("trEditor: CleanUp");
	std::vector<Panel*>::iterator it = panels.begin();

	while (it != panels.end()) {
		delete(*it);//is this legal? todo go to each panel
		it++;
	}
	panels.clear();

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(App->render->context);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return true;
}

void trEditor::Draw()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void trEditor::SetupInspectorWith(Mesh* mesh)
{
	inspector->SetDataFrom(mesh);
}


void trEditor::InfoFPSMS(float current_fps, float current_ms)
{
	(config != nullptr) ? config->FillChartFpsInfo(current_fps, current_ms) : Log("trEditor error: configuration == nullptr");
}

void trEditor::Log(const char * new_log)
{
	if (console != nullptr)
		console->AddLogToConsole(new_log);
	else {
		init_logs.push_back(new_log);
	}
}
