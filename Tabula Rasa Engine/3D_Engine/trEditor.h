#pragma once
#include "trModule.h"
#include "trDefs.h"

#include <vector>
#include <string>

class Panel;
class PanelAbout;
class PanelConfiguration;
class PanelConsole;

class trEditor : public trModule
{
public:

	trEditor();
	~trEditor();

	bool Init();
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	//todo implement this
	bool UserUsingMouse() const;
	bool UserUsingKeyboard() const;

	void InfoFPSMS(float current_fps, float current_ms);

	void Log(const char* new_log, const char* format = "", ...);

public:

	//panels
	PanelAbout* about = nullptr;
	PanelConfiguration* config = nullptr;
	PanelConsole* console = nullptr;

private:

	std::vector<std::string> init_logs;

	std::vector<Panel *> panels;

	bool user_using_mouse = false;
	bool user_using_keyboard = false;

	//default imgui demo window
	bool show_demo_window = false;

};