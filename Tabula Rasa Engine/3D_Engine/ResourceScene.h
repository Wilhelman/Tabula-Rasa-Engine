#ifndef __RESOURCE_SCENE_H__
#define __RESOURCE_SCENE_H__

#include "Resource.h"

class ResourceScene : public Resource
{

public:
	ResourceScene(UID uid);
	~ResourceScene();

	bool LoadInMemory() override;

	//void Save(Config& config) const override;
	//void Load(const Config& config) override;

};

#endif // __RESOURCE_SCENE_H__