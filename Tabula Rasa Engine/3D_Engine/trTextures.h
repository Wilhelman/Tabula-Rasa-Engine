#ifndef __trTEXTURES_H__
#define __trTEXTURES_H__

#include "ctModule.h"

#include <list>

struct SDL_Texture;
struct SDL_Surface;

class trTextures : public trModule
{
public:

	trTextures();

	// Destructor
	virtual ~trTextures();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const	Load(const char* path);
	bool				UnLoad(SDL_Texture* texture);
	SDL_Texture* const	LoadSurface(SDL_Surface* surface);
	void				GetSize(const SDL_Texture* texture, uint& width, uint& height) const;

public:

	std::list<SDL_Texture*> textures;
};


#endif // __trTEXTURES_H__