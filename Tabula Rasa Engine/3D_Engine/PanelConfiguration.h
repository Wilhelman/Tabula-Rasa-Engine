#ifndef __PANELCONFIGURATION_H__
#define __PANELCONFIGURATION_H__

#include "Panel.h"
#include "imgui.h"
#include <vector>

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
	void ShowAudio(trAudio * module);
	void ShowInput(trInput * module);
	void ShowWindow(trWindow * module);
	void ShowRenderer(trRenderer3D * module);

	void FillChartFpsInfo(float fps, float ms);

private:
	std::vector<float> chart_fps, chart_ms;

	char app_name[TITLE_ORG_MAX_LENGTH];
	char org_name[TITLE_ORG_MAX_LENGTH];

};

#endif// __PANELCONFIGURATION_H__