#ifndef __PANEL_INSPECTOR_H__
#define __PANEL_INSPECTOR_H__

#include "Panel.h"
#include "imgui.h"

#include <string>
#include <vector>

class Mesh;

class PanelInspector : public Panel
{
public:
	PanelInspector();
	virtual ~PanelInspector();

	void Draw() override;

	void PanelInspector::SetDataFrom(Mesh* mesh);

private:
	std::vector<Mesh*> meshes;

};

#endif// __PANEL_CONSOLE_H__