#ifndef __PANEL_RESOURCES_H__
#define __PANEL_RESOURCES_H__

#include "Panel.h"
#include <list>
#include <string>

struct Directory;

class PanelResources : public Panel
{
public:
	PanelResources();
	virtual ~PanelResources();

	void Draw() override;
	void DrawAssets(Directory* dir);

private:
	struct FilesTree
	{
		public:
			FilesTree(FilesTree* name, FilesTree* parent, std::list<FilesTree*> childs)
			{
				this->name = name;
				this->parent = parent;
				this->childs = childs;
			}

		public:
			FilesTree* name = nullptr;
			FilesTree* parent = nullptr;
			std::list<FilesTree*> childs;
	};
};

#endif// __PANELABOUT_H__