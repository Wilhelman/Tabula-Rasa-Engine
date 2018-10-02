#include "trPrimitives.h"

class PRay : public trPrimitive
{
public:
	PRay(math::vec origin, math::vec direction);
	void InnerRender() const;
public:
	math::vec origin;
	math::vec direction;
};