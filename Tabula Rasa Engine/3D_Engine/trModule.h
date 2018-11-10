// ----------------------------------------------------
// trModule.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __trMODULE_H__
#define __trMODULE_H__

#include <string>
#include "ParsonJson/parson.h"
#include "Event.h"

class trApp;

class trModule
{

public:

	trModule() : active(false)
	{}

	virtual ~trModule()
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(JSON_Object* config = nullptr)
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

	virtual void DrawDebug()
	{
	}

	virtual bool Load(const JSON_Object* config = nullptr)
	{
		return true;
	}

	virtual bool Save(JSON_Object* config = nullptr) const
	{
		return true;
	}

	virtual void TurnActive() {
		active = !active;
	}

	virtual void OnEventReceived(const Event& event) {}

public:

	std::string		name;
	bool		active = false;

};

#endif // __trMODULE_H__