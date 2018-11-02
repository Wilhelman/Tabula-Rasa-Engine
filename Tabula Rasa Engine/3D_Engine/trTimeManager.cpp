#include "trTimeManager.h"

trTimeManager::trTimeManager()
{
	game_clock = new trTimer();
	real_time_clock = new trTimer();
}

trTimeManager::~trTimeManager() { }

bool trTimeManager::Start()
{
	game_clock->Start();
	real_time_clock->Start();

	return true;
}

bool trTimeManager::CleanUp()
{
	RELEASE(real_time_clock);
	RELEASE(game_clock);

	return true;
}

float trTimeManager::ReadGameClockSec() const
{
	return game_clock->ReadSec();
}

float trTimeManager::ReadRealTimeClockSec() const
{
	return real_time_clock->ReadSec();
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

void trTimeManager::SetGameClockTimeScale(float time_scale)
{
	this->time_scale = time_scale;
}

void trTimeManager::PauseGameClock()
{
	game_clock->Pause();
}

void trTimeManager::PauseRealTimeClock()
{
	real_time_clock->Pause();
}

void trTimeManager::ReStartGameClock()
{
	game_clock->ReStart();
}