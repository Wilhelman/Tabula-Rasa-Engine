#include "trPrimitives.h"

class PRay;

class PGizmo : public trPrimitive
{
public:

	PGizmo(math::float3 origin);
	~PGizmo();

	void InnerRender() const;

private:

	math::float3 origin;
	PRay* x_arrow = nullptr;
	PRay* y_arrow = nullptr;
	PRay* z_arrow = nullptr;
	PRay* w_arrow = nullptr;

};