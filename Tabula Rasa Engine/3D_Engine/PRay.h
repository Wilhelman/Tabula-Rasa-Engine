#include "trPrimitives.h"

class PRay : public trPrimitive
{
public:
	PRay();
	~PRay();
	PRay(math::float3 origin, math::float3 destination, math::float4 color);
	void InnerRender() const;
public:
	math::float3 origin;
	math::float3 destination;
	math::float4 color;
};