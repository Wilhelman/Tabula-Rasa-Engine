#pragma once
#include "trModule.h"
#include "trDefs.h"

#include "SDL/include/SDL.h"

class trEditor : public trModule
{
public:

	trEditor();
	~trEditor();

	bool Init();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();


private:


};