#include "trPrimitives.h"

class PArrow : public trPrimitive
{
public:
	PArrow();
	PArrow(float x, float y, float z);
	void InnerRender() const;
public:
	math::vec origin;
	math::vec destination;
};