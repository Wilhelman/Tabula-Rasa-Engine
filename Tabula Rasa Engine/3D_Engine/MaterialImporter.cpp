#include "MaterialImporter.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "ComponentMaterial.h"
#include "trFileSystem.h"

#include "DevIL\include\ilut.h"
#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

MaterialImporter::MaterialImporter()
{
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		TR_LOG("trTexture: DevIL version is different ... exiting!");
	}

	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	TR_LOG("trTexture: Initializating DevIL ...");
}

MaterialImporter::~MaterialImporter()
{
}


bool MaterialImporter::Import(const char * file_path, std::string & output_file)
{
	return false;
}

bool MaterialImporter::Import(const void * buffer, uint size, std::string & output_file)
{
	return false;
}

Texture* MaterialImporter::LoadImageFromPath(const char * path)
{
	Texture* texture = new Texture();

	char* buffer = nullptr;

	App->file_system->ReadFromFile(path, &buffer);

	if (buffer == nullptr) {
		TR_LOG("Texture error loading file with path %s", path);
	}

	uint img_id = 0u;
	texture->id = 0u;

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

		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	
		//fill the rest of the texture info
		texture->path = path;
		texture->width = ilGetInteger(IL_IMAGE_WIDTH);
		texture->height = ilGetInteger(IL_IMAGE_HEIGHT);
		
	}
	else
	{
		error_num = ilGetError();
		TR_LOG("trTexture: Error loading the image - %i - %s from path - %s", error_num , iluErrorString(error_num),path);
	}

	ilDeleteImages(1, &img_id);

	if (texture->id != 0) {
		TR_LOG("trTexture: Texture created correctly");
		return texture;
	}
	else {
		return nullptr;
	}
		
}

void MaterialImporter::DeleteTextureBuffer(Texture * tex)
{
	ilDeleteImages(1, &tex->id);
}
