#include "trPrimitives.h"
#include "PArrow.h"

class PGizmo : public trPrimitive
{
public:

	PGizmo(math::vec origin);

	void InnerRender() const;

private:

	math::vec origin;
	PArrow x_arrow;
	PArrow y_arrow;
	PArrow z_arrow;

};