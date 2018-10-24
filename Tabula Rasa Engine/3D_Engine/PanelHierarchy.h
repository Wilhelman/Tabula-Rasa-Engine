#ifndef __PANEL_HIERARCHY_H__
#define __PANEL_HIERARCHY_H__

#include "Panel.h"

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy();
	virtual ~PanelHierarchy();

	void Draw() override;

private:

};

#endif// __PANEL_HIERARCHY_H__