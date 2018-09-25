
#include "trApp.h"
#include "trEditor.h"

trEditor::trEditor() : trModule()
{

}

// Destructor
trEditor::~trEditor()
{
}

// Called before render is available
bool trEditor::Init()
{
	TR_LOG("Init trEditor system");

	return true;
}

bool trEditor::PreUpdate(float dt)
{

	return true;
}

bool trEditor::Update(float dt)
{

	return true;
}

bool trEditor::PostUpdate(float dt)
{

	return true;
}

// Called before quitting
bool trEditor::CleanUp()
{
	TR_LOG("Cleaning trEditor");

	return true;
}