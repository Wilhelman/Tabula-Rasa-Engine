#ifndef __PANEL_RESOURCES_H__
#define __PANEL_RESOURCES_H__

#define REFRESH_TIME 2.0f

#include "Panel.h"
#include <string>
#include <vector>

struct Directory;
struct File;

class PanelResources : public Panel
{
public:
	PanelResources();
	virtual ~PanelResources();

	void Draw() override;
	void DrawAssets(Directory* dir);

private:
	float refresh_clock = 0.0f;
	bool import_clicked = false;
};

#endif// __PANELABOUT_H__