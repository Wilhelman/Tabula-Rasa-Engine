#ifndef __trTIMER_H__
#define __trTIMER_H__

#include "trDefs.h"


struct FormatHour;

class trTimer
{
public:
	struct FormatHour
	{
		uint hours = 0;
		uint min = 0;
		uint sec = 0;
	};


public:

	// Constructor
	trTimer();

	void Start();
	void SetStartTime(uint32 time);
	uint32 Read() const;
	float ReadSec();
	void Stop();
	void Pause();
	void ReStart();
	bool HasStarted() const;
	bool IsPaused() const;
	void SetScaleTime(float scale_time);
	void UpdateClock();
	FormatHour ReadFormatTime() const;

private:

	uint32	started_at = 0;
	uint32 paused_ticks = 0;
	float last_frame_time = 0;
	float scale_time = 0.0f;

	bool is_paused = false;
	bool has_started = false;


};

#endif //__ctTIMER_H__