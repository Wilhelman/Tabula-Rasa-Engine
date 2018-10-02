#include "trPrimitives.h"

class PPlane : public trPrimitive
{
public:
	PPlane();
	PPlane(math::vec normal, float d);
	void InnerRender() const;
public:
	
	float constant;
	math::vec position;
	math::vec normal;
};