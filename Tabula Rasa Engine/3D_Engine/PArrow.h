#include "trPrimitives.h"

class PArrow : public trPrimitive
{
public:
	PArrow();
	PArrow(math::vec origin, math::vec destination, math::float4 color);
	void InnerRender() const;
public:
	math::vec origin;
	math::vec destination;
	math::float4 color;
};