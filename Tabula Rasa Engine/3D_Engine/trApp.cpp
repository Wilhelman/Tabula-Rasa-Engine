#include <iostream> 

#include "trApp.h"
#include "trDefs.h"
#include "trLog.h"
#include "trWindow.h"
#include "trInput.h"
#include "trTextures.h"
#include "trRenderer3D.h"
#include "trCamera3D.h"
#include "trEditor.h"
#include "trHardware.h"
#include "trFileLoader.h"
#include "trTimeManager.h"
#include "trFileSystem.h"

#include "trMainScene.h"


// Constructor
trApp::trApp(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 1000 / 60;
	fps_counter = 0;

	input = new trInput();
	window = new trWindow();
	texture = new trTextures();
	render = new trRenderer3D();
	camera = new trCamera3D();
	main_scene = new trMainScene();
	hardware = new trHardware();
	file_loader = new trFileLoader();
	editor = new trEditor();
	time_manager = new trTimeManager();
	file_system = new trFileSystem();
	
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp

	AddModule(input);
	AddModule(window);
	AddModule(camera);
	AddModule(editor);
	AddModule(main_scene);
	AddModule(hardware);
	AddModule(file_loader);
	AddModule(time_manager);
	AddModule(file_system);
	AddModule(texture);

	// render last to swap buffer
	AddModule(render);

	//disable modules here
}

// Destructor
trApp::~trApp()
{
	// release modules
	
	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		RELEASE(*it);
	}

	modules.clear();
}

void trApp::AddModule(trModule* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool trApp::Awake()
{
	bool ret = true;

	JSON_Value* root_value = nullptr;
	root_value = json_parse_file("config.json");

	if (root_value != nullptr) {

		JSON_Object* root_obj = json_value_get_object(root_value);

		TR_LOG("trApp: config.json loaded correctly, iterating between modules ...");
		JSON_Object* app_obj = json_object_get_object(json_value_get_object(root_value), "app");
		this->SetTitle(json_object_get_string(app_obj, "title"));
		this->SetOrganization(json_object_get_string(app_obj, "organization"));
		this->SetFpsCap(json_object_get_number(app_obj, "framerate_cap"));
		cap_fps = json_object_get_boolean(app_obj, "cap_framerate");
		this->SetVersion(json_object_get_string(app_obj, "version"));

		for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
		{
			JSON_Object* module_obj = json_object_get_object(root_obj, (*it)->name.c_str());

			ret = (*it)->Awake(module_obj);
			if(!ret)
				TR_LOG("trApp: Error awakening in: %s", (*it)->name.c_str());
		}

		json_value_free(root_value);
	}
	else {

		TR_LOG("trApp: Error loading config.json file");

		for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
			ret = (*it)->Awake();

	}
		

	return ret;
}

// Called before the first frame
bool trApp::Start()
{
	bool ret = true;

	std::list<trModule*>::iterator it = modules.begin();
	
	while (it != modules.end() && ret == true)
	{
		if ((*it)->active == false) {
			it++;
			continue;
		}
		ret = (*it)->Start();
		it++;
	}

	return ret;
}

// Called each loop iteration
bool trApp::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (ret)
		ret = PreUpdate();

	if (ret)
		ret = DoUpdate();

	if (ret)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void trApp::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.f;

	if(run_time)
		time_manager->UpdateGameClock(dt);
	time_manager->UpdateRealTimeClock(dt);

	ms_timer.Start();
}

// ---------------------------------------------
void trApp::FinishUpdate()
{
	// Framerate calculations --

	if (want_to_save == true) {
		SaveNow();
		want_to_save = false;
	}

	if (want_to_load == true) {
		LoadNow();
		want_to_load = false;
	}

	++frames;
	++fps_counter;

	if (fps_timer.Read() > 1000.f)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	last_frame_ms = ms_timer.Read();

	editor->InfoFPSMS((float)last_fps, (float)ms_timer.Read(), frames);

	// Cap fps
	if (cap_fps && capped_ms > 0 && (last_frame_ms < capped_ms))
		SDL_Delay(capped_ms - last_frame_ms);

	if (!all_modules_loaded)
		all_modules_loaded = true;
}

// Call modules before each loop iteration
bool trApp::PreUpdate()
{
	bool ret = true;

	trModule* pModule = NULL;

	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		ret = (*it)->PreUpdate(dt);
	}

	return ret;
}

// Call modules on each loop iteration
bool trApp::DoUpdate()
{

	bool ret = true;

	trModule* pModule = NULL;

	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		(all_modules_loaded) ? ret = (*it)->Update(dt) : ret = (*it)->Update(0);
	}

	return ret;
}

// Call modules after each loop iteration
bool trApp::PostUpdate()
{
	bool ret = true;

	trModule* pModule = NULL;

	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		ret = (*it)->PostUpdate(dt);
	}

	//PERF_PEEK(ptimer);
	return ret;
}

// Called before quitting
bool trApp::CleanUp()
{
	bool ret = true;
	
	std::list<trModule*>::reverse_iterator it = modules.rbegin();

	while (it != modules.rend() && ret == true)
	{
		ret = (*it)->CleanUp();
		it++;
	}

	return ret;
}

// ---------------------------------------
int trApp::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* trApp::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* trApp::GetTitle() const
{
	return game_title.data();
}

void trApp::SetTitle(const char * title)
{
	game_title = title;
}

// ---------------------------------------
const char* trApp::GetOrganization() const
{
	return organization.data();
}

void trApp::RequestBrowser(const char * url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void trApp::SendTheCarrierPigeon(const Event & event)
{
	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end(); it++)
		(*it)->OnEventReceived(event);
}

void trApp::SetOrganization(const char * organization)
{
	this->organization = organization;
}

void trApp::SetVersion(const char * version)
{
	this->version = version;
}


uint trApp::GetFpsCap() const
{
	if (capped_ms > 0)
		return (uint)((1.0f / (float)capped_ms) * 1000.0f);
	else
		return 0;
}

const char * trApp::GetVersion() const
{
	return version.c_str();
}

void trApp::SetFpsCap(uint max_framerate)
{
	if (max_framerate > 0)
		capped_ms = 1000 / max_framerate;
	else
		capped_ms = 0;
}

void trApp::SwitchRunTime()
{
	run_time = !run_time;
}

bool trApp::IsRunTime() const
{
	return run_time;
}

void trApp::Save()
{
	want_to_save = true;
}

void trApp::Load()
{
	want_to_load = true;
}


bool trApp::LoadNow()
{
	TR_LOG("Loading ...");

	bool ret = true;

	JSON_Value* root_value = nullptr;
	root_value = json_parse_file("config.json");

	if (root_value != nullptr) {

		TR_LOG("config.json loaded correctly, iterating between modules ...");

		for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
		{
			trModule* pModule = (*it);

			if (pModule->active == false) {
				continue;
			}

			JSON_Object* module_obj = json_object_get_object(json_value_get_object(root_value), (*it)->name.c_str());

			ret = (*it)->Load(module_obj);
		}
		json_value_free(root_value);
	}
	else
		TR_LOG("trApp: Error loading config.json file");
	
	
	return ret;
}

bool trApp::SaveNow()
{

	TR_LOG("Saving ...");

	bool ret = true;

	JSON_Value* root_value = nullptr;
	root_value = json_parse_file("config.json");

	if (root_value == nullptr) {
		TR_LOG("trApp: Can't load config.json, generating a new one ...");
		root_value = json_value_init_object();
	}

	char *serialized_string = NULL;

	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		trModule* pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		JSON_Object* module_obj = json_object_get_object(json_value_get_object(root_value), (*it)->name.c_str());
		ret = (*it)->Save(module_obj);
	}
	
	serialized_string = json_serialize_to_string_pretty(root_value);
	puts(serialized_string);
	json_serialize_to_file(root_value, "config.json");
	json_free_serialized_string(serialized_string);
	json_value_free(root_value);

	return ret;
}