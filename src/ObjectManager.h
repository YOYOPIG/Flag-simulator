#pragma once
#include <vector>
#include "Entity.h"
#include <utility>
#include "AABBTree.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	static std::vector<Entity *> object_list;
	static AABBTree aabb_tree;
	static void qSort(std::vector<Entity *> list, bool _is_x);
	static void insertionSort(std::vector<std::pair<Entity *, glm::vec2>>, std::vector<Entity *> &list, bool _is_x);
};

