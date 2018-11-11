#ifndef __PANEL_RESOURCES_H__
#define __PANEL_RESOURCES_H__

#include "Panel.h"
#include <list>
#include <string>

class PanelResources : public Panel
{
public:
	PanelResources();
	virtual ~PanelResources();

	void Draw() override;
	void DrawAssets(std::string dir_name);
};

#endif// __PANELABOUT_H__