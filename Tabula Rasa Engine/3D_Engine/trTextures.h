#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "trModule.h"
#include "trDefs.h"

class Texture;

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

	Texture* LoadImageFromPath(const char* path);

private:
	Texture* tmp_tex = nullptr;
};

#endif // __TEXTURES_H__
