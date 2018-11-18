#include "MaterialImporter.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "ComponentMaterial.h"
#include "trFileSystem.h"

#include "trApp.h"
#include "trResources.h"
#include "ResourceTexture.h"

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
	ilShutDown();
}

bool MaterialImporter::Import(const char * file_path, std::string & output_file)
{
	return false;
}

bool MaterialImporter::Import(const char * file_path, const char* file_name, std::string & output_file, UID& uid_to_force)
{
	// TODO : solve this
	std::string final_path = file_path;
	final_path.append("/"); final_path.append(file_name);

	char* buffer = nullptr;

	uint file_size = App->file_system->ReadFromFile(final_path.c_str(), &buffer);

	if (buffer == nullptr) {
		TR_LOG("Texture error loading file with path %s", final_path.c_str());
		return false;
	}

	uint img_id = 0u;

	ILenum error_num;

	ilGenImages(1, &img_id);
	ilBindImage(img_id);

	if (!ilLoadL(IL_TYPE_UNKNOWN, buffer, file_size)) {
		TR_LOG("Texture error loading file with path %s", final_path.c_str());
		return false;
	}

	ilEnable(IL_FILE_OVERWRITE);

	ILuint size = 0;
	ILubyte* data = nullptr;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, NULL, 0);

	if (size > 0)
	{
		ilEnable(IL_FILE_OVERWRITE);
		data = new ILubyte[size];

		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			TR_LOG("Import texture with path [%s]: SUCCESS!", final_path.c_str());

			// Generate UUID for the resource
			if(uid_to_force == 0u)
				uid_to_force = App->GenerateNewUUID();

			// Saving file

			std::string tmp_str(L_MATERIALS_DIR);
			tmp_str.append("/");
			tmp_str.append(std::to_string(uid_to_force));
			tmp_str.append(".dds"); // adding our own format extension

			App->file_system->WriteInFile(tmp_str.c_str(), (char*)data, size);
			output_file = tmp_str;
		}
		RELEASE_ARRAY(buffer);
		RELEASE_ARRAY(data);
	}

	return true;
}

UID MaterialImporter::LoadImageFromPath(const char * path, UID uid_to_force)
{
	ResourceTexture* resource = (ResourceTexture*)App->resources->CreateNewResource(Resource::Type::TEXTURE, uid_to_force); // our tex
	if (resource == nullptr) { // resource already created!
		std::string tmp = path;
		const size_t last_slash = tmp.find_last_of("\\/");
		if (std::string::npos != last_slash)
			tmp.erase(0, last_slash + 1);

		return App->resources->Find(tmp.c_str());
	}


	uint img_id = 0u;
	resource->gpu_id = 0u;

	ILenum error_num;

	ilGenImages(1, &img_id);
	ilBindImage(img_id);

	char* buffer = nullptr;

	uint file_size = App->file_system->ReadFromFile(path, &buffer);

	if (buffer == nullptr) {
		TR_LOG("Texture error loading file with path %s", path);
		return false;
	}


	if (ilLoadL(IL_TYPE_UNKNOWN, buffer, file_size))
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

		glGenTextures(1, &resource->gpu_id);
		glBindTexture(GL_TEXTURE_2D, resource->gpu_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	
		//fill the rest of the texture info
		resource->SetExportedPath(path);
		resource->width = ilGetInteger(IL_IMAGE_WIDTH);
		resource->height = ilGetInteger(IL_IMAGE_HEIGHT);
		
	}
	else
	{
		error_num = ilGetError();
		TR_LOG("trTexture: Error loading the image - %i - %s from path - %s", error_num , iluErrorString(error_num),path);
	}

	ilDeleteImages(1, &img_id);

	if (resource->gpu_id != 0) {
		TR_LOG("trTexture: Texture created correctly");
		return resource->GetUID();
	}
	else {
		return 0u;
	}
}



void MaterialImporter::DeleteTextureBuffer(ResourceTexture * tex)
{
	ilDeleteImages(1, &tex->gpu_id);
}
