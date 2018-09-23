#ifndef __trMAINSCENE_H__
#define __trMAINSCENE_H__

#include "SDL/include/SDL.h"
#include "trModule.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include "SDL\include\SDL.h"
#include "SDL\include\SDL_opengl.h"
#include "pcg/pcg_variants.h"

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"

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
	void ShowMenuBar();

	void ShowRandomWindow(bool* p_open);
	void ShowMathGeoLibTestWindow(bool* p_open);

public:

	//testing zone
	Sphere sphere_1;
	Sphere sphere_2;

	Capsule capsule_1;
	Capsule capsule_2;

	AABB aabb_1;
	AABB aabb_2;

	OBB obb_1;
	OBB obb_2;

	Triangle tri_1;
	Triangle tri_2;

	Ray ray_1;

	Plane plane_1;
	Plane plane_2;
	//end testing zone

	//im_gui test
	bool show_demo_window = false;
	bool show_mgl_test_window = false;
	bool quit = false;

	ImVec4 clear_color = ImVec4(0.0f, 0.00f, 0.00f, 0.00f);
	ImGuiIO io;
	//end im_gui test

	//pcg_test
	double rand_num_range = 0;
	pcg32_random_t rng;
	pcg_state_setseq_64 rng_bound;
	uint32_t rand_min_max = 0;
	bool show_random_window = false;
	int min_num = 0;
	int max_num = 0;
	uint32_t bound = 0;
	int temp_min_num = 0;
	//pc_test end

};

#endif // __trRENDER_H__