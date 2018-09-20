// ----------------------------------------------------
 // Point class    -----------
 // ----------------------------------------------------

#ifndef __CTPOINT_H__
#define __CTPOINT_H__

#include "trDefs.h"
#include <math.h>

	template<class TYPE>
class trPoint
{
public:

	TYPE x, y;

	trPoint()
	{}

	trPoint(const trPoint<TYPE>& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	trPoint(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;
	}

	trPoint& create(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;

		return(*this);
	}

	// Math ------------------------------------------------
	trPoint operator -(const trPoint &v) const
	{
		ctPoint r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	trPoint operator + (const trPoint &v) const
	{
		trPoint r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	const trPoint& operator -=(const trPoint &v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const trPoint& operator +=(const trPoint &v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const trPoint& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const trPoint& v) const
	{
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	trPoint& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	trPoint& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const trPoint& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return sqrtf((fx*fx) + (fy*fy));
	}

	TYPE DistanceNoSqrt(const trPoint& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx*fx) + (fy*fy);
	}

	TYPE DistanceManhattan(const trPoint& v) const
	{
		return abs(v.x - x) + abs(v.y - y);
	}
};

typedef trPoint<int> iPoint;
typedef trPoint<float> fPoint;

#endif // __CTPOINT_H__
