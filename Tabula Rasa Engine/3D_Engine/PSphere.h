#include "trPrimitives.h"

class PSphere : public trPrimitive
{
public:

	PSphere();
	PSphere(math::vec position, float radius);
	void InnerRender() const;

public:

	math::vec position;
	float radius = 0.f;

};