#include "trAnimation.h"

#include "ResourceAnimation.h"
#include "GameObject.h"

#include "trDefs.h"
#include "trApp.h"
#include "trMainScene.h"

#include "trInput.h" //TODO: delete this


trAnimation::trAnimation()
{
	this->name = "Animation";
}

trAnimation::~trAnimation()
{}

bool trAnimation::Awake(JSON_Object* config)
{
	return true;
}

bool trAnimation::Start()
{

	return true;
}

// Called before quitting or switching levels
bool trAnimation::CleanUp()
{
	TR_LOG("Cleaning Animation");

	return true;
}

bool trAnimation::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		for (uint i = 0; i < animable_gos.size(); ++i)
		{
			ResourceAnimation::BoneTransformation* transform = animable_data_map.find(animable_gos[i])->second;
			
			if (transform)
			{
				float3 pos;
				float3 scale;
				Quat rot;
				animable_gos[i]->GetTransform()->GetLocalPosition(&pos, &scale, &rot);

				if(transform->positions.count > i)
					pos = float3(transform->positions.value[pos_count],
						   transform->positions.value[pos_count + 1],
						   transform->positions.value[pos_count + 2]);

				if (transform->scalings.count > i)
					scale = float3(transform->scalings.value[pos_count],
						     transform->scalings.value[pos_count + 1],
					         transform->scalings.value[pos_count + 2]);

				if (transform->rotations.count > i)
					rot = Quat(transform->rotations.value[rot_count],
						 transform->rotations.value[rot_count + 1],
						 transform->rotations.value[rot_count + 2],
						 transform->rotations.value[rot_count + 3]);

				animable_gos[i]->GetTransform()->Setup(pos, scale, rot);			
			}
		}

		pos_count += 3;
		rot_count += 4;
	}

	return true;
}

void trAnimation::SetAnimationGos(ResourceAnimation * res)
{

	for (uint i = 0; i < res->num_keys; ++i)
	{
		RecursiveGetAnimableGO(App->main_scene->GetRoot(), &res->bone_keys[i]);
	}

	int a = 0;

}

void trAnimation::RecursiveGetAnimableGO(GameObject * go, ResourceAnimation::BoneTransformation* bone_transformation)
{
	
	if (bone_transformation->bone_name.compare(go->GetName()) == 0) {
		animable_data_map.insert(std::pair<GameObject*, ResourceAnimation::BoneTransformation*>(go, bone_transformation));
		animable_gos.push_back(go);
		return;
	}

	for (std::list<GameObject*>::iterator it_childs = go->childs.begin(); it_childs != go->childs.end(); ++it_childs)
		RecursiveGetAnimableGO((*it_childs), bone_transformation);

}
