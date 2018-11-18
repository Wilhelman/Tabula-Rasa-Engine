#include "PanelConsole.h"
#include "ImGui/imgui.h"
#include "trApp.h"
#include "trWindow.h"

PanelConsole::PanelConsole(std::vector<std::string> init_logs) : Panel("Console", SDL_SCANCODE_1)
{
	active = true;

	std::vector<std::string>::iterator it = init_logs.begin();
	while (it != init_logs.end())
	{
		this->AddLogToConsole((*it).c_str());
		it++;
	}
}

PanelConsole::~PanelConsole()
{
	ClearLog();
}

void PanelConsole::Draw()
{

	ImGui::SetNextWindowPos(ImVec2(App->window->GetWidth() /2.f, App->window->GetHeight() - 200.f));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetWidth() /2,200.f));
	ImGui::Begin("Console", &active, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoFocusOnAppearing);

	ImGui::TextUnformatted(text_buffer.begin());

	if (have_to_scroll)
		ImGui::SetScrollHere(1.0f);

	have_to_scroll = false;
	
	ImGui::End();
}

void PanelConsole::AddLogToConsole(const char * log)
{
	text_buffer.empty() ? text_buffer.appendf(log) : text_buffer.appendf("\n%s",log);
	have_to_scroll = true;
}

void PanelConsole::ClearLog()
{
	text_buffer.clear();
}
