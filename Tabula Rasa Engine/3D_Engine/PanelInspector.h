#ifndef __PANEL_INSPECTOR_H__
#define __PANEL_INSPECTOR_H__

#include "Panel.h"
#include "imgui.h"

#include <string>
#include <vector>

class GameObject;

class PanelInspector : public Panel
{
public:
	PanelInspector();
	virtual ~PanelInspector();

	void Draw() override;

};

#endif// __PANEL_CONSOLE_H__