#ifndef __PANEL_HIERARCHY_H__
#define __PANEL_HIERARCHY_H__

#include "Panel.h"

class GameObject;

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy();
	virtual ~PanelHierarchy();

	void Draw() override;

	void ItemHovered(GameObject* go);

private:
	GameObject* dragged_go = nullptr;
	void DrawGameObject(GameObject* game_object);

};

#endif// __PANEL_HIERARCHY_H__