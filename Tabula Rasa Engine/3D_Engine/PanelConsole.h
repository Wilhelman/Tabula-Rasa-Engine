#ifndef __PANEL_CONSOLE_H__
#define __PANEL_CONSOLE_H__

#include "Panel.h"
#include "imgui.h"

class PanelConsole : public Panel
{
public:
	PanelConsole();
	virtual ~PanelConsole();

	void Draw() override;
	void AddLogToConsole(const char* log);
	void ClearLog();


private:

	ImGuiTextBuffer text_buffer;
	bool have_to_scroll = false;

};

#endif// __PANEL_CONSOLE_H__