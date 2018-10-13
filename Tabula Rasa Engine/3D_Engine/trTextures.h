#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "trModule.h"
#include "trDefs.h"


class trTextures : public trModule
{
public:

	trTextures();
	~trTextures();

	// Called before render is available
	bool Awake(JSON_Object* config = nullptr);

	// Called before the first frame
	bool Start();
	bool CleanUp();

	void LoadImageFromPath(const char* path);
};

#endif // __TEXTURES_H__
