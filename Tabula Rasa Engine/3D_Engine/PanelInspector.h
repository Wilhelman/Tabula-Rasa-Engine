#ifndef __PANEL_INSPECTOR_H__
#define __PANEL_INSPECTOR_H__

#include "Panel.h"
#include "imgui.h"

#include <string>
#include <vector>

class Mesh;
class Texture;

class PanelInspector : public Panel
{
public:
	PanelInspector();
	virtual ~PanelInspector();

	void Draw() override;

	void SetDataFrom(Mesh* mesh);
	void SetDataFrom(Texture* texture);
	void Clear();

private:
	std::vector<Mesh*> meshes;
	Texture* texture = nullptr; //for now only one tex
};

#endif// __PANEL_CONSOLE_H__