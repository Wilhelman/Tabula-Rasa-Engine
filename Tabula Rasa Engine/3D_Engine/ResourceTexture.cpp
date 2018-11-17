#include "ResourceTexture.h"

#include "trApp.h"
#include "trResources.h"
#include "MaterialImporter.h"

ResourceTexture::ResourceTexture(UID uid) : Resource(uid, Resource::Type::TEXTURE)
{
}

ResourceTexture::~ResourceTexture()
{
}

const char * ResourceTexture::GetFormatStr() const
{
	//TODO
	return nullptr;
}

bool ResourceTexture::LoadInMemory()
{

	return true;
}

bool ResourceTexture::ReleaseMemory()
{
	App->resources->material_importer->DeleteTextureBuffer(this);
	return true;
}
