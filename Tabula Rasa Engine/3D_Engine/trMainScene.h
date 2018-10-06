#ifndef __trMAINSCENE_H__
#define __trMAINSCENE_H__

#include "SDL/include/SDL.h"
#include "trModule.h"

class PPlane;
class PCube;

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

	void Draw();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(JSON_Value&) const;

private:
	PPlane* plane_test = nullptr;
	PCube* cube_test = nullptr;
};

#endif // __trRENDER_H__