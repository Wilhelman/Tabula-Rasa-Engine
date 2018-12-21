#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class ResourceMesh;
class ComponentBone;

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* embedded_game_object);
	ComponentMesh(GameObject* embedded_game_object, UID resource);
	~ComponentMesh();

	bool Save(JSON_Object* component_obj)const;
	bool Load(const JSON_Object* component_obj);

	bool SetResource(UID resource);

	void AttachBones(const GameObject* go);

	void DetachBones();

	uint CountAttachedBones() const;

	void RecursiveFindBones(const GameObject * go, std::vector<ComponentBone*>& found) const;

	

public:
	uint root_bones_uid = 0;
	const GameObject* root_bones = nullptr;

	std::vector<ComponentBone*> attached_bones;
};

#endif // __COMPONENT_MATERIAL_H__