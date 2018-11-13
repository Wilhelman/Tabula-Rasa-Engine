#ifndef __trAPP_H__
#define __trAPP_H__

#include <list>
#include <string>
#include "trModule.h"
#include "trTimer.h"
#include "ParsonJson/parson.h"
#include "trLog.h"
#include "SDL/include/SDL.h"
#include "MathGeoLib/Algorithm/Random/LCG.h"
#include "pcg/pcg_variants.h"

// Modules
class trWindow;
class trInput;
class trRenderer3D;
class trCamera3D;
class trMainScene;
class trEditor;
class trHardware;
class trFileLoader;
class trTimeManager;
class trFileSystem;
class trResources;

class trApp
{
public:

	// Constructor
	trApp(int argc, char* args[]);

	// Destructor
	virtual ~trApp();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(trModule* module);

	// Exposing some properties for reading aka getters
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	uint GetFpsCap() const;
	const char* GetVersion()const;

	// Usefull requests
	void RequestBrowser(const char* url)const;
	void SendTheCarrierPigeon(const Event& event);

	// Setters
	void SetTitle(const char* title);
	void SetOrganization(const char* organization);
	void SetVersion(const char* version);
	void SetFpsCap(uint max_framerate);

	void SwitchRunTime();
	bool IsRunTime()const;

	// Load / Save
	void Save();
	void Load();

private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	bool LoadNow();
	bool SaveNow();

public:

	// Modules
	trWindow*			window = nullptr;
	trInput*			input = nullptr;
	trRenderer3D*		render = nullptr;
	trCamera3D*			camera = nullptr;
	trMainScene*		main_scene = nullptr;
	trEditor*			editor = nullptr;
	trHardware*			hardware = nullptr;
	trFileLoader*		file_loader = nullptr;
	trTimeManager*		time_manager = nullptr;
	trFileSystem*		file_system = nullptr;
	trResources*		resources = nullptr;

	//LCG*				gen_uuid = nullptr;
	pcg32_random_t		gen_uuid;

private:

	std::list<trModule*>modules;
	int					argc = 0;
	char**				args = nullptr;

	std::string			organization;
	std::string			game_title;
	std::string			version;

	bool				all_modules_loaded = false;
	bool				want_to_save, want_to_load = false;

	bool				run_time = false;

	//fps/ms
	trTimer				ms_timer;
	trTimer				fps_timer;
	Uint32				frames = 0;
	float				dt = 0.f;
	int					fps_counter = 0;
	int					last_frame_ms = 0;
	int					last_fps = 0;
	int					capped_ms = 0;
	bool				cap_fps = true;

};

extern trApp* App;

#endif