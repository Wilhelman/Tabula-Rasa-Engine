#include "trDefs.h"
#include "trLog.h"
#include "trApp.h"
#include "trRender.h"
#include "trTextures.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

trTextures::trTextures() : trModule()
{
	name = "textures";
}

// Destructor
trTextures::~trTextures()
{}

// Called before render is available
bool trTextures::Awake(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool trTextures::Start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

// Called before quitting
bool trTextures::CleanUp()
{
	LOG("Freeing textures and Image library");
	
	for(std::list<SDL_Texture*>::iterator it= textures.begin(); it != textures.end(); it++ )
	{
		SDL_DestroyTexture(*it);
	}

	textures.clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const trTextures::Load(const char* path)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	if (texture != NULL)
		textures.push_back(texture);
	else
		return NULL;


	return texture;
}

// Unload texture


bool trTextures::UnLoad(SDL_Texture* texture)
{
	std::list<SDL_Texture*>::const_iterator item;

	for (item = textures.begin(); item != textures.end(); ++item)
	{
		if (texture == (*item))
		{
			SDL_DestroyTexture(*item);
			textures.remove(*item);
			return true;
		}
	}

	return false;
}


// Translate a surface into a texture
SDL_Texture* const trTextures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.push_back(texture);
	}

	return texture;
}

// Retrieve size of a texture
void trTextures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height);
}