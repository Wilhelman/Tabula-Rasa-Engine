#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"

struct Texture;

class ComponentMaterial : public Component
{
public:

	ComponentMaterial(GameObject* embedded_game_object);
	~ComponentMaterial(); //todo clean the texture here

	const Texture* GetTexture()const;
	void SetTexture(Texture* texture);

public:

	Texture* texture = nullptr;

};

#endif // __COMPONENT_MATERIAL_H__