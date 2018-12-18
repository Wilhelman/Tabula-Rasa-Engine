#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "trModule.h"
#include "trDefs.h"
#include "MathGeoLib\MathGeoLib.h"


class trAnimation : public trModule
{
public:

	trAnimation();
	~trAnimation();

	// Called before render is available
	bool Awake(JSON_Object* config = nullptr);

	// Called before the first frame
	bool Start();
	bool CleanUp();


};

#endif // __ANIMATION_H__
