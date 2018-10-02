#include "trPrimitives.h"

class PFrustum : public trPrimitive
{
public:
	PFrustum();
	void InnerRender() const;
public:
	math::vec position;
};