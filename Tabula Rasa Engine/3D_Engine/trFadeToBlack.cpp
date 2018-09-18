#include <math.h>
#include "trApp.h"
#include "trFadeToBlack.h"
#include "trRender.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "trWindow.h"
#include "trLog.h"


trFadeToBlack::trFadeToBlack()
{
	name = "fadeToBlack";
}

trFadeToBlack::~trFadeToBlack()
{}

bool trFadeToBlack::Awake(pugi::xml_node& conf)
{
	uint winWidth, winHeight;

	App->win->GetWindowSize(winWidth, winHeight);

	screen = { 0, 0,  (int)winWidth * (int)App->win->GetScale(), (int)winHeight * (int)App->win->GetScale() };

	return true;
}

// Load assets
bool trFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	lvlName = "";
	F1 = false;
	return true;
}

// Update: draw background
bool trFadeToBlack::PostUpdate(float dt)
{
	bool ret = true;
	if (current_step == FadeStep::NONE_FADE)
		return ret;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case FadeStep::FADE_TO_BLACK:
	{
		if (now >= total_time)
		{

			if (moduleOn != nullptr && moduleOff != nullptr) {
				this->moduleOff->CleanUp();
				this->moduleOff->active = false;

				this->moduleOn->Start();
				this->moduleOn->active = true;

			}
			
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = FadeStep::FADE_FROM_BLACK;

		}
	} break;

	case FadeStep::FADE_FROM_BLACK:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = FadeStep::NONE_FADE;
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return ret;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool trFadeToBlack::FadeToBlack(float time)
{
	lvlName = "";
	bool ret = false;
	if (current_step == FadeStep::NONE_FADE)
	{
		current_step = FadeStep::FADE_TO_BLACK;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}

bool trFadeToBlack::FadeToKnowBlack(std::string lvlToFade, float time)
{
	lvlName = lvlToFade;
	bool ret = false;
	if (current_step == FadeStep::NONE_FADE)
	{
		current_step = FadeStep::FADE_TO_BLACK;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}

bool trFadeToBlack::FadeToBlackBetweenModules(trModule* module_off, trModule* module_on, float time, bool andLoad)
{
	bool ret = false;

	have_to_load = andLoad;
	moduleOff = module_off;
	moduleOn = module_on;

	if (current_step == FadeStep::NONE_FADE)
	{
		current_step = FadeStep::FADE_TO_BLACK;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}

bool trFadeToBlack::FadeIsOver() {
	bool ret = true;

	if (current_step == FadeStep::NONE_FADE)
		ret = true;
	else
		ret = false;

	return ret;
}