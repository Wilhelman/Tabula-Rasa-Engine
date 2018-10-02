#include "trPrimitives.h"

class PSphere : public trPrimitive
{
public:
	PSphere();
	PSphere(float radius);
	void InnerRender() const;
public:
	float radius;
};