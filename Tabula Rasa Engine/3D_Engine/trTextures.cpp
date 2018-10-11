#include "trTextures.h"
#include "trApp.h"
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

bool trTextures::Awake(JSON_Object* config)
{
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
	    iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) 
	{
		TR_LOG("trTexture: DevIL version is different ... exiting!");
		return false;
	}

	ilInit();
	iluInit();
	ilutInit();
	TR_LOG("trTexture: Initializating DevIL ...");

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
	uint img_id = 0u;
	uint texture_id = 0u;

	ILenum error_num;

	ilGenImages(1, &img_id);
	ilBindImage(img_id);

	if (ilLoadImage(path))
	{
		// Flip the image if needed
		ILinfo img_info;
		iluGetImageInfo(&img_info);
		if (img_info.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		// Convert the image to RGBA format -> can also be RGB
		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			error_num = ilGetError();
			TR_LOG("trTexture: Error converting the image - %i - %s", error_num, iluErrorString(error_num));
		}

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	}
	else
	{
		error_num = ilGetError();
		TR_LOG("trTexture: Error loading the image - %i - %s from path - %s", error_num , iluErrorString(error_num),path);
	}

	ilDeleteImages(1, &img_id);

	if (texture_id != 0) {
		TR_LOG("trTexture: Texture created correctly");
		App->render->SetTextureID(texture_id);
	 }
		
}

const uint trTextures::GetTextureID()
{
	return texture_id;
}


