#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "trModule.h"
#include "trDefs.h"
#include "DevIL\include\il.h"
#include "Glew\include\GL\glew.h"

struct ImageTexture
{
	GLuint index = 0;
	GLubyte image = 0;
	int width = 0;
	int height = 0;
};
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

	uint LoadImageFromPath(const char* path);

private:
	ImageTexture loaded_texture;
};

#endif // __TEXTURES_H__
