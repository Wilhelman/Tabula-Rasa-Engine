#include "trPrimitives.h"

class PArrow;

class PGizmo : public trPrimitive
{
public:

	PGizmo(math::vec origin);
	~PGizmo();

	void InnerRender() const;

private:

	math::vec origin;
	PArrow* x_arrow = nullptr;
	PArrow* y_arrow = nullptr;
	PArrow* z_arrow = nullptr;
	PArrow* w_arrow = nullptr;

};