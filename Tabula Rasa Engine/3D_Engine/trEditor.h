#pragma once
#include "trModule.h"
#include "trDefs.h"

#include <vector>

class Panel;
class PanelAbout;

class trEditor : public trModule
{
public:

	trEditor();
	~trEditor();

	bool Init();
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void Draw();
	bool UsingMouse() const;
	bool UsingKeyboard() const;

public:
	PanelAbout* about = nullptr;

private:

	std::vector<Panel *> panels;

	bool capture_mouse = false;
	bool capture_keyboard = false;
	bool show_demo_window = false;

};