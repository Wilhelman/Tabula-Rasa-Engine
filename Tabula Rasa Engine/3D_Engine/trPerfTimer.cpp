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
	is_paused = false;
	has_started = true;
	started_at = SDL_GetPerformanceCounter();
	paused_time = 0;
}

// ---------------------------------------------
double trPerfTimer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

// ---------------------------------------------
double trPerfTimer::ReadSec() const
{
	double time = 0;

	if (has_started)
	{
		if (is_paused)
			time = paused_time;
		else
			time = (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
	}

	return time;
}

// ---------------------------------------------
uint64 trPerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}

void trPerfTimer::Pause()
{
	if (has_started && !is_paused)
	{
		is_paused = true;
		paused_time = ReadSec() - started_at;
		started_at = 0;
	}
}

void trPerfTimer::ReStart()
{
	if (has_started && is_paused)
	{
		is_paused = false;
		started_at = ReadSec() - paused_time;
		paused_time = 0;
	}
}

void trPerfTimer::Stop()
{
	has_started = false;
	is_paused = false;
	started_at = 0;
	paused_time = 0;
}