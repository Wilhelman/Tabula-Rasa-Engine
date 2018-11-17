#pragma once
#include "trModule.h"
#include "trDefs.h"

#include <vector>
#include <string>

#include "ImGuizmo/ImGuizmo.h"

class Panel;
class PanelAbout;
class PanelConfiguration;
class PanelConsole;
class PanelInspector;
class PanelHierarchy;
class PanelResources;
class PanelControl;

class Mesh;
class Texture;

class GameObject;

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

	void OnEventReceived(const Event& event);

	void Draw();

	void InfoFPSMS(float current_fps, float current_ms, int frames);

	void Log(const char* new_log);

	GameObject* GetSelected()const;
	void SetSelected(GameObject* selected);

	void DisplayGuizmos();

public:

	//panels
	PanelAbout* about = nullptr;
	PanelConfiguration* config = nullptr;
	PanelConsole* console = nullptr;
	PanelInspector* inspector = nullptr;
	PanelHierarchy* hierarchy = nullptr;
	PanelResources* resources = nullptr;
	PanelControl* control = nullptr;

	ImGuizmo::MODE guizmo_mode = ImGuizmo::MODE::WORLD;

private:

	std::vector<std::string> init_logs;

	std::vector<Panel*> panels;

	GameObject* selected = nullptr;

	//default imgui demo window
	bool show_demo_window = false;
	bool have_to_quit = false;
	bool show_imgui = true;

	// ImGuizmo stuff
	ImGuizmo::OPERATION guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
};