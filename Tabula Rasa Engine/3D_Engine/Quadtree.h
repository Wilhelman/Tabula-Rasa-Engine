#ifndef __QUAD_TREE_H__
#define __QUAD_TREE_H__

#include "MathGeoLib/MathGeoLib.h"

#include <list>

class GameObject;

class QuadtreeNode {

	// Constructor
public:
	QuadtreeNode(AABB limit);
	~QuadtreeNode();

	void Insert(GameObject* go);
	bool IsLeaf() const;
	void GenerateChilds();


private:
	AABB box;
	QuadtreeNode* parent = nullptr;
	QuadtreeNode* childs[4];
	std::list<GameObject*> objects_inside;
};

class Quadtree {

	// Constructor
public:
	Quadtree();
	~Quadtree();

	// Operations
public:

	void Create(AABB limits);
	void Insert(GameObject* go);

	/*

	Create(AABB limits)
	Clear()
	Insert(GameObject*)
	Remove(GameObject*)
	Intersect(vector<GameObject*>& , PRIMITIVE)

	*/


public:
	QuadtreeNode* root_node = nullptr;

};


#endif // __QUAD_TREE_H__