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
}

// ---------------------------------------------

void trTimer::SetStartTime(uint32 time) {
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
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}