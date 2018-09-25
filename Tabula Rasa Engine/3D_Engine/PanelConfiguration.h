#ifndef __PANELCONFIGURATION_H__
#define __PANELCONFIGURATION_H__

// Editor Panel for all Module's configuration
#include "Panel.h"
#include "imgui.h"
#include <vector>

#define FPS_LOG_SIZE 100

class trModule;
class trWindow;
class trInput;
class trAudio;
class trRenderer3D;
class trCamera3D;
class trHardware;
class trEditor;

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration();
	virtual ~PanelConfiguration();

	void Draw() override;

	bool InitModuleDraw(trModule* module);
	void DrawApplication();
	void DrawModuleHardware(trHardware * module);
	void DrawModuleAudio(trAudio * module);
	void DrawModuleInput(trInput * module);
	void DrawModuleWindow(trWindow * module);
	void DrawModuleRenderer(trRenderer3D * module);
	void DrawModuleCamera(trCamera3D * module);
	void AddInput(const char* entry);
	void AddFPS(float fps, float ms);

private:
	ImGuiTextBuffer input_buf;
	bool need_scroll = false;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
};

#endif// __PANELCONFIGURATION_H__