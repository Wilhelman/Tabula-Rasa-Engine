#ifndef __trTIME_MANAGER_H__
#define __trTIME_MANAGER_H__

#include "trModule.h"
#include "trDefs.h"
#include "trTimer.h"

class trTimeManager : public trModule
{
public:

	trTimeManager();
	~trTimeManager();

	bool Start();
	bool CleanUp();

	// Read only
	double ReadGameClockSec() const;
	double ReadRealTimeClockSec() const;
	float GetGameClockTimeScale() const;
	uint GetFrameCount() const;
	float GetGameDt() const;
	float GetRealTimeDt() const;

	// Set
	void SetGameClockTimeScale(float time_scale);
	void UpdateGameClock(float dt);
	void UpdateRealTimeClock(float dt);

	void PauseGameClock();
	void ReStartGameClock();

private:

	// attributes
	uint frame_count = 0;

	double real_sec_time = 0.0f;
	double game_sec_time = 0.0f;

	float time_scale = 1.0f;

	float real_time_dt = 0.0f;
	float dt = 0;

	bool is_game_clock_paused = false;
	bool is_real_time_clock_paused = false;
};

#endif // __trTIME_MANAGER_H__