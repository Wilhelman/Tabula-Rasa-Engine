#include "trPrimitives.h"
#include "PGizmo.h"

class PGrid : public trPrimitive
{
public:

	PGrid();
	~PGrid();
	void InnerRender() const;

public:

	PGizmo * gizmo = nullptr;

};