#ifndef __trTIMER_H__
#define __trTIMER_H__

#include "trDefs.h"

class trTimer
{
public:

	// Constructor
	trTimer();

	void Start();
	void SetStartTime(uint32 time);
	uint32 Read() const;
	float ReadSec() const;

private:
	uint32	started_at = 0;
};

#endif //__ctTIMER_H__