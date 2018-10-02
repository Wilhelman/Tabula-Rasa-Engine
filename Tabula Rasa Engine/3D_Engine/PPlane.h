#include "trPrimitives.h"

class PPlane : public trPrimitive
{
public:
	PPlane();
	PPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	math::vec normal;
	float constant;
};