#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

struct Texture;

class ComponentMaterial : public Component
{
public:

	ComponentMaterial(GameObject* embedded_game_object);
	~ComponentMaterial(); //todo clean the texture here

	void SetAmbientColor(float4 ambient_color);
	const float4 GetAmbientColor()const;

	const Texture* GetTexture()const;
	void SetTexture(Texture* texture);

private:

	Texture* texture = nullptr;
	float4 ambient_color = float4::zero;

};

#endif // __COMPONENT_MATERIAL_H__