#include "Quadtree.h"
#include "trDefs.h"

#define BUCKET_SIZE 3

Quadtree::Quadtree()
{
}

Quadtree::~Quadtree()
{
	RELEASE(root_node);
}

void Quadtree::Create(AABB limits)
{
	root_node = new QuadtreeNode(limits);
}

void Quadtree::Insert(GameObject * go)
{
}

// ------------------------------------- NODE ---------------------------------------------------- \\

QuadtreeNode::QuadtreeNode(AABB limit)
{
	box = limit;
}

QuadtreeNode::~QuadtreeNode()
{
	// TODO: iterate and delete
}

void QuadtreeNode::Insert(GameObject * go)
{
	if (objects_inside.size() < BUCKET_SIZE) {
		objects_inside.push_back(go);
	}
	else {
		GenerateChilds();
	}
}

bool QuadtreeNode::IsLeaf() const
{
	return (objects_inside.size() > 0);
}

void QuadtreeNode::GenerateChilds()
{
	// Setting up
	float3 min_point = box.minPoint;
	float3 max_point = box.maxPoint;
	float width = math::Abs(min_point.x - max_point.x);
	float height = math::Abs(min_point.y - max_point.y);
	float depth = math::Abs(min_point.z - max_point.z);

	// First child
	float3 min_point_c1 = min_point;
	float3 max_point_c1 = min_point;
	max_point_c1.x += width / 2.f;
	max_point_c1.y += height / 2.f;
	max_point_c1.z += depth / 2.f;
	AABB child_1_box = AABB(min_point_c1, max_point_c1);
	childs[0] = new QuadtreeNode(child_1_box);
}
