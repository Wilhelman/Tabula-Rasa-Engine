#ifndef __trTIME_MANAGER_H__
#define __trTIME_MANAGER_H__

#include "trModule.h"
#include "trDefs.h"
#include "trTimer.h"

class trTimeManager : public trModule
{

public:



public:

	trTimeManager();
	~trTimeManager();

	bool Start();
	bool CleanUp();

	// Read only
	float ReadGameClockSec() const;
	float ReadRealTimeClockSec() const;
	float GetGameClockTimeScale() const;
	uint GetFrameCount() const;
	float GetGameDt() const;
	float GetRealTimeDt() const;

	// Set
	void SetGameClockTimeScale(float time_scale);

	void PauseGameClock();
	void PauseRealTimeClock();
	void ReStartGameClock();

private:

	// attributes
	uint frame_count = 0;
	float game_sec_time = 0.0f;
	float time_scale = 0.0f;
	float dt = 0.0f;
	float real_sec_time = 0.0f;
	float real_time_dt = 0.0f;

	//clocks 
	trTimer* game_clock = nullptr;
	trTimer* real_time_clock = nullptr;
};

#endif // __trTIME_MANAGER_H__