#ifndef __TRPERFTIMER_H__
#define __TRPERFTIMER_H__

#include "trDefs.h"

class trPerfTimer
{
public:

	// Constructor
	trPerfTimer();

	void Start();
	double ReadMs() const;
	uint64 ReadTicks() const;

private:
	uint64	started_at = 0;
	static uint64 frequency;
};

#endif //__CTPERFTIMER_H__