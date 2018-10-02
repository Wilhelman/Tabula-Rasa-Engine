#include "trPrimitives.h"


class PCylinder : public trPrimitive
{
public:
	PCylinder();
	PCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};