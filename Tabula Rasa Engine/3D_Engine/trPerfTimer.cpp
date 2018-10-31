// ----------------------------------------------------
// trPerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "trPerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 trPerfTimer::frequency = 0;

// ---------------------------------------------
trPerfTimer::trPerfTimer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}

// ---------------------------------------------
void trPerfTimer::Start()
{
	stopped = false;
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
double trPerfTimer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

// ---------------------------------------------
double trPerfTimer::ReadSec() const
{
	if (stopped)
		return stopped_time;
	else
		return (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

// ---------------------------------------------
uint64 trPerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}

void trPerfTimer::Stop()
{
	stopped_time = ReadSec();
	stopped = true;
}