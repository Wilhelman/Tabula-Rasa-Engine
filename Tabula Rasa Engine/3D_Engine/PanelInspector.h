#ifndef __PANEL_INSPECTOR_H__
#define __PANEL_INSPECTOR_H__

#include "Panel.h"
#include "ImGui/imgui.h"
#include "trDefs.h"

#include <string>
#include <vector>

class GameObject;

class PanelInspector : public Panel
{
public:
	PanelInspector();
	virtual ~PanelInspector();

	void Draw() override;

private:
	char go_name[GAMEOBJECT_MAX_LENGTH];
	std::string btn_guizmo_mode;
	float current_anim_time[2];
};

#endif// __PANEL_CONSOLE_H__