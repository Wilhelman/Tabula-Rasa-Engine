#include "trPrimitives.h"

class PArrow : public trPrimitive
{
public:
	PArrow();
	~PArrow();
	PArrow(math::float3 origin, math::float3 destination, math::float4 color);
	void InnerRender() const;
public:
	math::float3 origin;
	math::float3 destination;
	math::float4 color;
};