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
	scale_time = 1.0f;
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
	float time = 0.0f;

	if (has_started)
	{
		if (is_paused)
			time = paused_ticks / 1000.0f;
		else
		{
			time = SDL_GetTicks() - started_at;
			time = current_time + (time - current_time) * scale_time;
			time /= 1000.0f;
		}
	}

	return time;
}

trTimer::FormatHour trTimer::ReadFormatTime() const
{
	float time = 0.0f;

	if (has_started)
	{
		if (is_paused)
			time = paused_ticks / 1000.0f;
		else
		{
			time = SDL_GetTicks() - started_at;
			time = current_time + (time - current_time) * scale_time;
			time /= 1000.0f;
		}
	}

	FormatHour ret_format;
	ret_format.hours = time / 3600;
	time = (uint)time % 3600;
	ret_format.min = time / 60.0f;
	time = (uint)time % 60;
	ret_format.sec = time;

	return ret_format;
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

void trTimer::SetScaleTime(float scale_time)
{
	if (scale_time < 0.0f)
		this->scale_time = 0.0f;
	else if (scale_time > 3.0f)
		this->scale_time = 3.0f;
	else
		this->scale_time = scale_time;
}

void trTimer::UpdateClock()
{
	current_time = SDL_GetTicks() - started_at;
}