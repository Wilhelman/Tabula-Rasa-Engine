#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"

class ResourceTexture : public Resource
{

public:

	enum Format {
		UNKNOWN,

		COLOR_INDEX,
		RGB,
		RGBA,
		BGR,
		BGRA,
		LUMINANCE
	};

public:
	ResourceTexture();
	virtual ~ResourceTexture();

	const char* GetFormatStr() const;

	bool LoadInMemory() override;

	void Save(Config& config) const override;
	void Load(const Config& config) override;

public:
	uint width = 0;
	uint height = 0;
	uint depth = 0;
	uint bpp = 0;
	uint mips = 0;
	uint bytes = 0;
	uint gpu_id = 0;

	Format format = unknown;
};

#endif // __RESOURCE_TEXTURE_H__