#include "trTextures.h"
#include "trApp.h"
#include "trEditor.h"
#include "trRenderer3D.h"

#include "DevIL\include\ilut.h"
#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

trTextures::trTextures()
{
}

trTextures::~trTextures()
{
}

bool trTextures::Awake(pugi::xml_node &)
{
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
	    iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) 
	{
		App->editor->Log("DevIL version is different...exiting!\n");
		return false;
	}

	ilInit();
	App->editor->Log("Initializating DevIL...\n");

	return true;
}

bool trTextures::Start()
{
	return true;
}

bool trTextures::CleanUp()
{
	return true;
}

void trTextures::LoadImageFromPath(const char * path)
{
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	if (ilLoadImage(path))
		App->editor->Log("Image succesfully loaded from path");
	else
		App->editor->Log("Cannot load image from path...");

	ILenum error;
	error = ilGetError();

	while ((error = ilGetError()) != IL_NO_ERROR) 
		TR_LOG("%d: %s/n", error, iluErrorString(error));

	loaded_texture.index = ilutGLBindTexImage();
	loaded_texture.image = image;
	loaded_texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	loaded_texture.height = ilGetInteger(IL_IMAGE_HEIGHT);

	App->editor->Log("Freeing image...");
	ilDeleteImages(1, &image);

	App->render->SetTexture(&loaded_texture);
}


