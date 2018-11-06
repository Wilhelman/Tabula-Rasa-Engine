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
};

#endif// __PANEL_CONSOLE_H__