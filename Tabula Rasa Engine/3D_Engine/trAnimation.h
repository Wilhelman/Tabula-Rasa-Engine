#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "trModule.h"
#include "trDefs.h"
#include "MathGeoLib\MathGeoLib.h"
#include "ResourceAnimation.h"
#include "trTimer.h"

#include <vector>
#include <map>

class GameObject;

class trAnimation : public trModule
{
public:

	trAnimation();
	~trAnimation();

	// Called before render is available
	bool Awake(JSON_Object* config = nullptr);

	// Called before the first frame
	bool Start();
	bool CleanUp();
	bool Update(float dt);

	void SetAnimationGos(ResourceAnimation* res);

	void RecursiveGetAnimableGO(GameObject* go, ResourceAnimation::BoneTransformation* bone_transformation);
	void MoveAnimationForward(float t);
	bool FindBoundingKeys(float& pos, float& scale, float& rotation, float t);

	std::vector<GameObject*> animable_gos;
	std::map<GameObject*, ResourceAnimation::BoneTransformation*> animable_data_map;

private:

	uint pos_count = 0;
	uint scale_count = 0;
	uint rot_count = 0;

	trTimer anim_timer;
	bool time_start = false;


};

#endif // __ANIMATION_H__
