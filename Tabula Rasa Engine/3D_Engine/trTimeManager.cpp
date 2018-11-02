#include "trTimeManager.h"


trTimeManager::trTimeManager() { }

trTimeManager::~trTimeManager() { }

bool trTimeManager::Start() { return true; }


// ---------- Getters ----------

double trTimeManager::ReadGameClockSec() const
{
	return game_sec_time;
}

double trTimeManager::ReadRealTimeClockSec() const
{
	return real_sec_time;
}

trTimeManager::FormatHour trTimeManager::ReadGameHourFormat() const
{
	double tmp_time = game_sec_time;

	FormatHour ret_format;
	ret_format.hours = tmp_time / 3600;
	tmp_time = (uint)tmp_time % 3600;
	ret_format.min = tmp_time / 60.0f;
	tmp_time = (uint)tmp_time % 60;
	ret_format.sec = tmp_time;

	return ret_format;
}

trTimeManager::FormatHour trTimeManager::ReadRealTimeHourFormat() const
{
	double tmp_time = real_sec_time;

	FormatHour ret_format;
	ret_format.hours = tmp_time / 3600;
	tmp_time = (uint)tmp_time % 3600;
	ret_format.min = tmp_time / 60.0f;
	tmp_time = (uint)tmp_time % 60;
	ret_format.sec = tmp_time;

	return ret_format;
}

float trTimeManager::GetGameClockTimeScale() const
{
	return time_scale;
}

uint trTimeManager::GetFrameCount() const
{
	return frame_count;
}

float trTimeManager::GetGameDt() const
{
	return dt;
}

float trTimeManager::GetRealTimeDt() const
{
	return real_time_dt;
}

// ---------- Setters ----------

void trTimeManager::SetGameClockTimeScale(float time_scale)
{
	this->time_scale = time_scale;
}

void trTimeManager::UpdateGameClock(float dt)
{
	if (is_game_clock_paused)
	{
		this->dt = 0.0f;
		time_step = false;
	}
	else
		this->dt = dt;

	game_sec_time += (double)(this->dt * time_scale);
	frame_count++;

	if (time_step)
		is_game_clock_paused = true;
}

void trTimeManager::UpdateRealTimeClock(float dt)
{
	real_time_dt = dt;
	real_sec_time += (double)(real_time_dt);
}

// ---------- Time management ----------

void trTimeManager::PauseGameClock()
{
	is_game_clock_paused = true;
}

void trTimeManager::ReStartGameClock()
{
	is_game_clock_paused = false;
}

void trTimeManager::StepGameClock()
{
	is_game_clock_paused = false;
	time_step = true;
}