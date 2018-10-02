#include "trPrimitives.h"

class PCube : public trPrimitive
{
public:
	PCube();
	PCube(math::vec position, math::vec size);
	void InnerRender() const;
private:
	math::vec position;
	math::vec size;
};