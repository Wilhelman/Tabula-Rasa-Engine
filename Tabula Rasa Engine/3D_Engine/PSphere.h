#include "trPrimitives.h"

class PSphere : public trPrimitive
{
public:
	PSphere();
	PSphere(float radius, unsigned int rings, unsigned int sectors);
	void InnerRender() const;
public:
	float radius;
};