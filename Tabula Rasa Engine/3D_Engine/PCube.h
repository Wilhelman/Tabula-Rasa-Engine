#include "trPrimitives.h"

class PCube : public trPrimitive
{
public:
	PCube();
	PCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	math::vec size;
};