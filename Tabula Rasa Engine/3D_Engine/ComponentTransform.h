#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "MathGeoLib/MathGeoLib.h"
#include "Component.h"

class ComponentTransform : public Component
{
public:

	ComponentTransform(GameObject* embedded_game_object);
	ComponentTransform(GameObject* embedded_game_object, const float3& translation, const float3& scale, const Quat& rotation);
	~ComponentTransform();

	void Setup(const float3& translation, const float3& scale, const Quat& rotation);
	const float3& GetTranslation()const;
	const float3& GetScale()const;
	const Quat& GetRotation()const;

private:

	float3 translation = float3::zero;
	float3 scale = float3::zero;
	Quat rotation = Quat::identity;

};

#endif // __COMPONENT_MATERIAL_H__