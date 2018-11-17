#ifndef __PANEL_CONTROL_H__
#define __PANEL_CONTROL_H__

#include "Panel.h"
#include "ImGui/imgui.h"
#include "trDefs.h"

class PanelControl : public Panel
{
public:
	PanelControl();
	virtual ~PanelControl();

	void Draw() override;

};

#endif// __PANEL_CONTROL_H__