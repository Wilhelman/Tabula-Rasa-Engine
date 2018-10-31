// ----------------------------------------------------
// ctTimer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "trTimer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
trTimer::trTimer()
{
}

// ---------------------------------------------
void trTimer::Start()
{
	started_at = SDL_GetTicks();

	has_started = true;
	is_paused = false;
	paused_ticks = 0;
}

// ---------------------------------------------

void trTimer::SetStartTime(uint32 time) 
{
	started_at = SDL_GetTicks() + time;
}

// ---------------------------------------------
uint32 trTimer::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float trTimer::ReadSec() const
{
	double time = 0;

	if (has_started)
	{
		if (is_paused)
			time = paused_ticks / 1000.0f;
		else
			time = (SDL_GetTicks() - started_at) / 1000.0f;
	}

	return time;
}

void trTimer::Stop()
{
	has_started = false;
	is_paused = false;
	started_at = 0;
	paused_ticks = 0;
}

void trTimer::Pause()
{
	if (has_started && !is_paused)
	{
		is_paused = true;
		paused_ticks = SDL_GetTicks() - started_at;
		started_at = 0;
	}
}

void trTimer::ReStart()
{
	if (has_started && is_paused)
	{
		is_paused = false;
		started_at = SDL_GetTicks() - paused_ticks;
		paused_ticks = 0;
	}
}

bool trTimer::HasStarted() const
{
	return has_started;
}

bool trTimer::IsPaused() const
{
	return is_paused;
}