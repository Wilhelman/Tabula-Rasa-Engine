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
	double ReadSec() const;
	uint64 ReadTicks() const;
	void Pause();
	void ReStart();
	void Stop();

private:

	uint64	started_at = 0;
	static uint64 frequency;
	bool is_paused = false;
	double paused_time = 0;
	bool has_started = false;

};

#endif //__CTPERFTIMER_H__