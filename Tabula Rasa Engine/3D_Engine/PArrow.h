#include "trPrimitives.h"

class PArrow : public trPrimitive
{
public:
	PArrow();
	PArrow(math::vec destination, math::vec color);
	void InnerRender() const;
public:
	math::vec origin;
	math::vec destination;
	math::vec color;
};