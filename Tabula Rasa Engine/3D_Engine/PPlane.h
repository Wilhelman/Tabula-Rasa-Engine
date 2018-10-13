#include "trPrimitives.h"

class PPlane : public trPrimitive
{
public:

	PPlane();
	PPlane(math::vec normal, float d);
	void InnerRender() const;

public:
	
	float constant = 0.f;
	math::vec position;
	math::vec normal;

};