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
	void GetDirectoryFiles(Directory* dir_to_compare, std::vector<File>& compare_files_vec);

private:
	float refresh_clock = 0.0f;
};

#endif// __PANELABOUT_H__