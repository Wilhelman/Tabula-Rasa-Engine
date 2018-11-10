#ifndef __trMAINSCENE_H__
#define __trMAINSCENE_H__

#include "trModule.h"
#include "Quadtree.h"

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

	void DrawDebug();

	// Called before quitting
	bool CleanUp();

	void ClearScene();

	void Draw();

	// Load / Save
	bool Load(const JSON_Object* config = nullptr);
	bool Save(JSON_Object* config = nullptr) const;

	GameObject* GetRoot()const;

	void InsertGoInQuadtree(GameObject* go);
	void EraseGoInQuadtree(GameObject* go);

	void CollectDinamicGOs(std::vector<GameObject*>& dinamic_vector);

	void ReDoQuadtree();

	void TestAgainstRay(LineSegment line_segment);

	GameObject* CreateGameObject(GameObject* parent);
	GameObject* CreateGameObject(const char* name, GameObject* parent = nullptr);

private:

	PGrid* grid = nullptr;

	GameObject* root = nullptr;
	
	std::list<GameObject*> static_go;
	std::list<GameObject*> dinamic_go;
	
public:
	Quadtree quadtree;
	GameObject* main_camera = nullptr;
	

};

#endif // __trRENDER_H__