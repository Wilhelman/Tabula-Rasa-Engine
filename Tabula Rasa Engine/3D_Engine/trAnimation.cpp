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
	counter = 1;
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
		anim_timer += dt * 2.0f;
		MoveAnimationForward(anim_timer);
	}
	/*	time_start = true;

	if (time_start)
		anim_timer += refresh_time;

	if (time_start && anim_timer >= (refresh_time * counter))
	{
		MoveAnimationForward(anim_timer);
		counter++;
	}*/
	
	return true;
}

void trAnimation::SetAnimationGos(ResourceAnimation * res)
{
	for (uint i = 0; i < res->num_keys; ++i)
		RecursiveGetAnimableGO(App->main_scene->GetRoot(), &res->bone_keys[i]);
	
	refresh_time = 1.0f / 12.0f;
}

void trAnimation::RecursiveGetAnimableGO(GameObject * go, ResourceAnimation::BoneTransformation* bone_transformation)
{
	if (bone_transformation->bone_name.compare(go->GetName()) == 0) 
	{
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

			float next_time = 0.0f;
			float prev_time = 0.0f;

			// -------- FINDING NEXT AND PREVIOUS TRANSFORMATIONS IN RELATION WITH THE GIVEN TIME (t) --------

			// Finding next and previous positions	
			if (transform->positions.count > i)
			{				
				for (uint j = 0; j < transform->positions.count; j += 3)
				{
					if (prev_pos != nullptr && next_pos != nullptr) // if prev and next postions have been found we stop
					{
						float time_interval = next_time - prev_time;
						time_pos_percentatge = (t - prev_time) / time_interval;
						break;
					}

					if (t == transform->positions.time[j]) // in this case interpolation won't be done
					{
						prev_pos = &transform->positions.value[j];
						next_pos = prev_pos;
						break;
					}

					if (transform->positions.time[j] > t) // prev and next postions have been found
					{
						next_time = transform->positions.time[j];
						next_pos = &transform->positions.value[j];

						prev_pos = &transform->positions.value[j - 3];
						prev_time = transform->positions.time[j - 3];
					}
				}
			}

			// Finding next and previous scalings
			if (transform->scalings.count > i)
			{
				next_time = 0.0f;
				prev_time = 0.0f;

				for (uint j = 0; j < transform->scalings.count; j += 3)
				{
					if (prev_scale != nullptr && next_scale != nullptr) // if prev and next scalings have been found we stop
					{
						float time_interval = next_time - prev_time;
						time_scale_percentatge = (t - prev_time) / time_interval;
						break;
					}

					if (t == transform->scalings.time[j]) // in this case interpolation won't be done
					{
						prev_scale = &transform->scalings.value[j];
						next_scale = prev_scale;
						break;
					}

					if (transform->scalings.time[j] > t) // prev and next scalings have been found
					{
						next_time = transform->scalings.time[j];
						next_scale = &transform->scalings.value[j];

						prev_scale = &transform->scalings.value[j - 3];
						prev_time = transform->scalings.time[j - 3];
					}
				}
			}
				
			// Finding next and previous rotations
			if (transform->rotations.count > i)
			{
				next_time = 0.0f;
				prev_time = 0.0f;

				for (uint j = 0; j < transform->rotations.count; j += 4)
				{
					if (prev_rot != nullptr && next_rot != nullptr) // if prev and next rotations have been found we stop
					{
						float time_interval = next_time - prev_time;
						time_rot_percentatge = (t - prev_time) / time_interval; 
						break;
					}

					if (t == transform->rotations.time[j]) // in this case interpolation won't be done
					{
						prev_rot = &transform->rotations.value[j];
						next_rot = prev_rot;
						break;
					}

					if (transform->rotations.time[j] > t) // prev and next rotations have been found
					{
						next_time = transform->rotations.time[j];
						next_rot = &transform->rotations.value[j];

						prev_rot = &transform->rotations.value[j - 4];
						prev_time = transform->rotations.time[j - 4];
					}
				}
			}

			// -------- INTERPOLATIONS CALCULATIONS --------

			// Interpolating positions
			if (prev_pos != nullptr && next_pos != nullptr && prev_pos != next_pos)
			{
				float3 prev_pos_lerp(prev_pos[0], prev_pos[1], prev_pos[2]);
				float3 next_pos_lerp(next_pos[0], next_pos[1], next_pos[2]);
				pos = float3::Lerp(prev_pos_lerp, next_pos_lerp, time_pos_percentatge);
			}
			else if (prev_pos != nullptr && prev_pos == next_pos)
				pos = float3(prev_pos[0], prev_pos[1], prev_pos[2]);

			// Interpolating scalings
			if (prev_scale != nullptr && next_scale != nullptr && prev_scale != next_scale)
			{
				float3 prev_scale_lerp(prev_scale[0], prev_scale[1], prev_scale[2]);
				float3 next_scale_lerp(next_scale[0], next_scale[1], next_scale[2]);
				scale = float3::Lerp(prev_scale_lerp, next_scale_lerp, time_scale_percentatge);
			}
			else if (prev_scale != nullptr && prev_scale == next_scale)
				scale = float3(prev_scale[0], prev_scale[1], prev_scale[2]);

			// Interpolating rotations
			if (prev_rot != nullptr && next_rot != nullptr && prev_rot != next_rot)
			{
				Quat prev_rot_lerp(prev_rot[0], prev_rot[1], prev_rot[2], prev_rot[3]);
				Quat next_rot_lerp(next_rot[0], next_rot[1], next_rot[2], next_rot[3]);
				rot = Quat::Slerp(prev_rot_lerp, next_rot_lerp, time_rot_percentatge);
			}
			else if (prev_rot != nullptr && prev_rot == next_rot)
				rot = Quat(prev_rot[0], prev_rot[1], prev_rot[2], prev_rot[3]);

			// Setting up final interpolated transform in current bone (gameobject)
			animable_gos[i]->GetTransform()->Setup(pos, scale, rot);	
		}
	}
}

bool trAnimation::FindBoundingKeys(float & pos, float & scale, float & rotation, float t)
{
	return true;
}