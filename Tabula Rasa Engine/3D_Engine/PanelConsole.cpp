#include "PanelConsole.h"
#include "imgui.h"

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

}

void PanelConsole::Draw()
{
	ImGui::Begin("Console", &active);
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
