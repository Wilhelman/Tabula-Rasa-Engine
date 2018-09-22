#ifndef __trMAINSCENE_H__
#define __trMAINSCENE_H__

#include "SDL/include/SDL.h"
#include "trModule.h"
//#include "Primitive.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include "SDL\include\SDL.h"
#include "SDL\include\SDL_opengl.h"
#include "pcg/pcg_variants.h"

struct PhysBody3D;
struct PhysMotor3D;

class trMainScene : public trModule
{
public:

	trMainScene();

	// Destructor
	virtual ~trMainScene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:
	void ShowTheFirstTabulaRasaWindow();
	void ShowExampleMenuFile();
	void ShowRandomWindow(bool* p_open);

private:

	//math::Plane p(math::float3(0.1f,0.1f,0.1f));
	//Primitives::Plane p = Plane(0, 1, 0, 0);

public:
	PhysBody3D* sensor;

	bool show_demo_window = false;
	bool quit = false;

	ImVec4 clear_color = ImVec4(0.0f, 0.00f, 0.00f, 0.00f);
	ImGuiIO io;
	double random_num_range = 0;
	pcg32_random_t rng;
	pcg_state_setseq_64 bound;
	uint32_t rand_min_max = 0;
	bool show_random_window = false;
};

#endif // __trRENDER_H__