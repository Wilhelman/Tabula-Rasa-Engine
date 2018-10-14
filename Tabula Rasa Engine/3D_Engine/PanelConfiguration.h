#ifndef __PANELCONFIGURATION_H__
#define __PANELCONFIGURATION_H__

#include "Panel.h"
#include "imgui.h"
#include "imgui_defs.h"
#include <vector>

#include "trTimer.h"

class trModule;
class trWindow;
class trInput;
class trAudio;
class trRenderer3D;
class trCamera3D;
class trHardware;

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration();
	virtual ~PanelConfiguration();

	void Draw() override;

	bool SetUpCollapsingHeader(trModule* module);

	void ShowApplication();
	void ShowHardware(trHardware * module);
	void ShowInput(trInput * module);
	void ShowWindow(trWindow * module);
	void ShowRenderer(trRenderer3D * module);

	void FillChartFpsInfo(float fps, float ms, int frames);

private:
	std::vector<float> chart_fps, chart_ms;

	char app_name[TITLE_MAX_LENGTH];
	char org_name[TITLE_MAX_LENGTH];
	char title[TITLE_MAX_LENGTH];

	trTimer mem_timer;
	std::vector<float> mem_list;
	uint index = 0;

};

#endif// __PANELCONFIGURATION_H__