#include <iostream> 

#include "trApp.h"
#include "trDefs.h"
#include "trLog.h"
#include "trWindow.h"
#include "trInput.h"
#include "trTextures.h"
#include "trRenderer3D.h"
#include "trAudio.h"
#include "trCamera3D.h"
#include "trEditor.h"
#include "trHardware.h"
#include "trFileLoader.h"

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
	audio = new trAudio();
	camera = new trCamera3D();

	main_scene = new trMainScene();
	editor = new trEditor();
	hardware = new trHardware();
	file_loader = new trFileLoader();
	
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(window);
	AddModule(audio);
	AddModule(camera);

	AddModule(main_scene);
	AddModule(editor);
	AddModule(hardware);
	AddModule(file_loader);
	AddModule(texture);

	// render last to swap buffer
	AddModule(render);

	//the start one

	//disable modules here

	//PERF_PEEK(ptimer);
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
		game_title = json_object_get_string(app_obj, "title");
		organization = json_object_get_string(app_obj, "organization");
		capped_ms = 1000 / json_object_get_number(app_obj, "framerate_cap");
		cap_fps = json_object_get_boolean(app_obj, "cap_framerate");

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

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void trApp::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
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

	if (fps_timer.Read() >= 1000)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	last_frame_ms = ms_timer.Read();

	// cap fps
	if (cap_fps) {
		if (capped_ms > 0 && (last_frame_ms < capped_ms))
			SDL_Delay(capped_ms - last_frame_ms);
	}

	editor->InfoFPSMS((float)last_fps, (float)last_frame_ms); //todo ms not working correctly

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

// ---------------------------------------
const char* trApp::GetOrganization() const
{
	return organization.data();
}

void trApp::RequestBrowser(const char * url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void trApp::SetOrganization(const char * organization)
{
	this->organization = organization;
}

uint trApp::GetFpsCap() const
{
	if (capped_ms > 0)
		return (uint)((1.0f / (float)capped_ms) * 1000.0f);
	else
		return 0;
}

void trApp::SetFpsCap(uint max_framerate)
{
	if (max_framerate > 0)
		capped_ms = 1000 / max_framerate;
	else
		capped_ms = 0;
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