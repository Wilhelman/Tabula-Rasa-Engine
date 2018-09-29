#include <iostream> 

#include "trApp.h"
#include "trDefs.h"
#include "trLog.h"
#include "trWindow.h"
#include "trInput.h"
#include "trRenderer3D.h"
#include "trAudio.h"
#include "trCamera3D.h"
#include "trEditor.h"
#include "trHardware.h"

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
	win = new trWindow();
	render = new trRenderer3D();
	audio = new trAudio();
	camera = new trCamera3D();

	main_scene = new trMainScene();
	editor = new trEditor();
	hardware = new trHardware();
	
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(audio);
	AddModule(camera);

	AddModule(main_scene);
	AddModule(editor);
	AddModule(hardware);

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

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;

	config = LoadConfig(config_file);


	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		game_title = app_config.child("title").child_value();
		organization =app_config.child("organization").child_value();

		//cap = app_config.attribute("framerate_cap").as_uint();
		//todo check this when json is implemented
		//if (cap > 0)
			//capped_ms = 1000 / cap;

	}

	if (ret == true)
	{

		std::list<trModule*>::iterator it = modules.begin();

		while (it != modules.end() && ret == true)
		{
			ret = (*it)->Awake(config.child((*it)->name.data()));
			
			it++;
		}
	}

//	PERF_PEEK(ptimer);

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
pugi::xml_node trApp::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if (result == NULL) {
		//LOG("Could not load xml file config.xml. pugi error: %s", result.description());
	}
	else
		ret = config_file.child("config");

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
	if (capped_ms > 0 && (last_frame_ms < capped_ms))
		SDL_Delay(capped_ms - last_frame_ms);

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
	/*

	JSON_Value *schema = json_parse_string("{\"name\":\"\"}");
	JSON_Value *user_data = json_parse_file("data.json");
	char buf[256];
	const char *name = NULL;
	if (user_data == NULL || json_validate(schema, user_data) != JSONSuccess) {

	sprintf_s(buf, 256, "Nombre de prueba");
	user_data = json_value_init_object();
	json_object_set_string(json_object(user_data), "name", buf);
	json_serialize_to_file(user_data, "user_data.json");

	}
	name = json_object_get_string(json_object(user_data), "name");
	char log_test[256];
	sprintf_s(log_test, 256, "Nombre: %s",name);
	editor->Log(log_test);
	json_value_free(schema);
	json_value_free(user_data);*/
	bool ret = true;

	JSON_Value *root_value = json_value_init_object();
	char *serialized_string = NULL;


	serialized_string = json_serialize_to_string_pretty(root_value);
	puts(serialized_string);
	json_serialize_to_file(root_value, "config.json");
	json_free_serialized_string(serialized_string);
	json_value_free(root_value);

	return ret;
}

bool trApp::SaveNow()
{

	App->editor->Log("Saving ...");

	bool ret = true;

	JSON_Value *root_value = json_value_init_object();
	char *serialized_string = NULL;

	trModule* pModule = NULL;
	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		ret = (*it)->Save(*root_value);
	}
	
	serialized_string = json_serialize_to_string_pretty(root_value);
	puts(serialized_string);
	json_serialize_to_file(root_value, "config.json");
	json_free_serialized_string(serialized_string);
	json_value_free(root_value);

	return ret;
}