#include "trAnimation.h"

#include "trDefs.h"
#include "trApp.h"


trAnimation::trAnimation()
{
	this->name = "Animation";
}

trAnimation::~trAnimation()
{}

bool trAnimation::Awake(JSON_Object* config)
{
	return true;
}

bool trAnimation::Start()
{

	return true;
}

// Called before quitting or switching levels
bool trAnimation::CleanUp()
{
	TR_LOG("Cleaning Animation");

	return true;
}