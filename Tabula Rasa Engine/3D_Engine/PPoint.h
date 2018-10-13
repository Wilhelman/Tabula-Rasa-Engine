#include "trPrimitives.h"

class PPoint : public trPrimitive
{
public:

	PPoint(math::vec pos, math::float4 color);
	void InnerRender() const;

public:

	math::vec pos;
	math::float4 color;

};