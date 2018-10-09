// ----------------------------------------------------
// trModule.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __trMODULE_H__
#define __trMODULE_H__

struct PhysBody3D;

#include <string>
#include "PugiXml\src\pugixml.hpp"
#include "ParsonJson\parson.h"

class trApp;
struct Collider;
class UIElement;
enum UI_State;
enum ComboType;

class trModule
{
public:

	trModule() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate(float dt)
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(JSON_Value&)
	{
		return true;
	}

	virtual bool Save(JSON_Value&) const
	{
		return true;
	}

	// Callbacks ---
	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	virtual void OnUITrigger(UIElement* elementTriggered, UI_State ui_state) {}

	virtual void TurnActive() {
		active = !active;
	}

public:

	std::string		name;
	bool		active = false;

};

#endif // __trMODULE_H__