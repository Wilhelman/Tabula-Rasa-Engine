#pragma once
#include "trModule.h"
#include "trDefs.h"

#include <vector>
#include <string>

class Panel;
class PanelAbout;
class PanelConfiguration;
class PanelConsole;
class PanelInspector;

class Mesh;

class trEditor : public trModule
{
public:

	trEditor();
	~trEditor();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void Draw();

	void SetupInspectorWith(Mesh* mesh);

	void InfoFPSMS(float current_fps, float current_ms);

	void Log(const char* new_log);

public:

	//panels
	PanelAbout* about = nullptr;
	PanelConfiguration* config = nullptr;
	PanelConsole* console = nullptr;
	PanelInspector* inspector = nullptr;

private:

	std::vector<std::string> init_logs;

	std::vector<Panel *> panels;

	bool user_using_mouse = false;
	bool user_using_keyboard = false;

	//default imgui demo window
	bool show_demo_window = false;

	bool have_to_quit = false;

};