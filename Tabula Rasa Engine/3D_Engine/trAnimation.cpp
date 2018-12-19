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

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		anim_timer += dt;
		time_start = true;
	}

	if (time_start)
	{
		MoveAnimationForward(anim_timer);
		time_start = false;
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

void trAnimation::MoveAnimationForward(float t)
{
	for (uint i = 0; i < animable_gos.size(); ++i)
	{
		ResourceAnimation::BoneTransformation* transform = animable_data_map.find(animable_gos[i])->second;

		if (transform)
		{
			float3 pos, scale;
			Quat rot;

			animable_gos[i]->GetTransform()->GetLocalPosition(&pos, &scale, &rot);

			float* prev_pos = nullptr;
			float* next_pos = nullptr;
			float time_pos_percentatge = 0.0f;

			float* prev_scale = nullptr;
			float* next_scale = nullptr;
			float time_scale_percentatge = 0.0f;

			float* prev_rot = nullptr;
			float* next_rot = nullptr;
			float time_rot_percentatge = 0.0f;

			float max_time = 0.0f;
			float min_time = 0.0f;

			// -------- FINDING NEXT AND PREVIOUS TRANSFORMATION IN RELATIONS WITH THE GIVEN TIME (t) --------

			// Finding next and previous positions	
			if (transform->positions.count > i)
			{				
				for (uint j = 0; j < transform->positions.count; j += 3)
				{
					if (prev_pos != nullptr && next_pos != nullptr) // if prev and next frames have been found we stop
					{
						float time_interval = max_time - min_time;
						time_pos_percentatge = (t - min_time) / time_interval;
						break;
					}

					if (t == transform->positions.time[j]) // in this case interpolation won't be done
					{
						prev_pos = &transform->positions.value[j];
						next_pos = prev_pos;
						break;
					}

					if (transform->positions.time[j] > t) // next frame has been found
					{
						max_time = transform->positions.time[j];
						next_pos = &transform->positions.value[j];

						prev_pos = &transform->positions.value[j - 3];
						min_time = transform->positions.time[j - 3];
					}
				}
			}

			// Finding next and previous scalings
			if (transform->scalings.count > i)
			{
				max_time = 0.0f;
				min_time = 0.0f;

				for (uint j = 0; j < transform->scalings.count; j += 3)
				{
					if (prev_scale != nullptr && next_scale != nullptr) // if prev and next frames have been found we stop
					{
						float time_interval = max_time - min_time;
						time_scale_percentatge = (t - min_time) / time_interval;
						break;
					}

					if (t == transform->scalings.time[j]) // in this case interpolation won't be done
					{
						prev_scale = &transform->scalings.value[j];
						next_scale = prev_scale;
						break;
					}

					if (transform->scalings.time[j] > t) // next frame has been found
					{
						max_time = transform->scalings.time[j];
						next_scale = &transform->scalings.value[j];

						prev_scale = &transform->scalings.value[j - 3];
						min_time = transform->scalings.time[j - 3];
					}
				}
			}
				
			// Finding next and previous rotations
			if (transform->rotations.count > i)
			{
				max_time = 0.0f;
				min_time = 0.0f;

				for (uint j = 0; j < transform->rotations.count; j += 4)
				{
					if (prev_rot != nullptr && next_rot != nullptr) // if prev and next frames have been found we stop
					{
						float time_interval = max_time - min_time;
						time_rot_percentatge = (t - min_time) / time_interval;
						break;
					}

					if (t == transform->rotations.time[j]) // in this case interpolation won't be done
					{
						prev_rot = &transform->rotations.value[j];
						next_rot = prev_rot;
						break;
					}

					if (transform->rotations.time[j] > t) // next frame has been found
					{
						max_time = transform->rotations.time[j];
						next_rot = &transform->rotations.value[j];

						prev_rot = &transform->rotations.value[j - 4];
						min_time = transform->rotations.time[j - 4];
					}
				}
			}

			// -------- INTERPOLATIONS CALCULATIONS --------

			// Interpolating positions
			if (prev_pos != nullptr && next_pos != nullptr)
				{
					float3 prev_pos_lerp(prev_pos[0], prev_pos[1], prev_pos[2]);

					if (prev_pos != next_pos)
					{
						float3 next_pos_lerp(next_pos[0], next_pos[1], next_pos[2]);
						pos = float3::Lerp(prev_pos_lerp, next_pos_lerp, time_pos_percentatge);
					}
				}

			// Interpolating scalings
			if (prev_scale != nullptr && next_scale != nullptr)
			{
				float3 prev_scale_lerp(prev_scale[0], prev_scale[1], prev_scale[2]);

				if (prev_scale != next_scale)
				{
					float3 next_scale_lerp(next_scale[0], next_scale[1], next_scale[2]);
					scale = float3::Lerp(prev_scale_lerp, next_scale_lerp, time_scale_percentatge);
				}
			}

			// Interpolating rotations
			if (prev_rot != nullptr && next_rot != nullptr)
				{
					Quat prev_rot_lerp(prev_rot[0], prev_rot[1], prev_rot[2], prev_rot[3]);

					if (prev_rot != next_rot)
					{
						Quat next_rot_lerp(next_rot[0], next_rot[1], next_rot[2], next_rot[3]);
						rot = Quat::Slerp(prev_rot_lerp, next_rot_lerp, time_rot_percentatge);
					}
				}

			// Setting up final interpolated transform
			animable_gos[i]->GetTransform()->Setup(pos, scale, rot);	
		}
	}
}

bool trAnimation::FindBoundingKeys(float & pos, float & scale, float & rotation, float t)
{
	return true;
}