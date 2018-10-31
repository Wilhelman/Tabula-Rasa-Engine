#ifndef __trMAINSCENE_H__
#define __trMAINSCENE_H__

#include "trModule.h"


class GameObject;
class PGrid;

class trMainScene : public trModule
{
public:

	trMainScene();

	// Destructor
	virtual ~trMainScene();

	// Called before render is available
	bool Awake(JSON_Object* config = nullptr);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	void ClearScene();

	void Draw();

	// Load / Save
	bool Load(JSON_Object* config = nullptr);
	bool Save(JSON_Object* config = nullptr) const;

	GameObject* GetRoot()const;

	GameObject* CreateGameObject(GameObject* parent);
	GameObject* CreateGameObject(const char* name, GameObject* parent = nullptr);

private:

	PGrid* grid = nullptr;
	std::string* default_mesh = nullptr;

	GameObject* root = nullptr;

};

#endif // __trRENDER_H__