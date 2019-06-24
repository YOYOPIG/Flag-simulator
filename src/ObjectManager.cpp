#include "ObjectManager.h"


std::vector<Entity *> ObjectManager::object_list;
AABBTree ObjectManager::aabb_tree;


ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

void ObjectManager::qSort(std::vector<Entity*> list, bool _is_x)
{
	if (_is_x) {
		;
	}
	else {
		;
	}
}

void ObjectManager::insertionSort(std::vector<std::pair<Entity *, glm::vec2>> unsort_list, std::vector<Entity*> &list, bool _is_x)
{
	std::vector<Entity *> sorted_list;
	sorted_list.reserve(unsort_list.size());
	if (_is_x) {
		for (int i = 0; i < list.size(); ++i) {
			sorted_list.at(i) = unsort_list.at(i).first;
			for (int j = i; i > 0; --j) {
				if (sorted_list.at(j)->rigid.getAABB()->_left_down_back.x < sorted_list.at(j - 1)->rigid.getAABB()->_left_down_back.x) {
					Entity *tmp = sorted_list.at(j);
					sorted_list.at(j) = sorted_list.at(i);
					sorted_list.at(i) = tmp;
					unsort_list.at(i).second.x--;//not correct
				}
				else {
					break;
				}
			}
		}
	}
	else {
		for (int i = 0; i < unsort_list.size(); ++i) {
			sorted_list.at(i) = unsort_list.at(i).first;
			for (int j = i; i > 0; --j) {
				if (sorted_list.at(j)->rigid.getAABB()->_left_down_back.y < sorted_list.at(j - 1)->rigid.getAABB()->_left_down_back.y) {
					Entity *tmp = sorted_list.at(j);
					sorted_list.at(j) = sorted_list.at(i);
					sorted_list.at(i) = tmp;
					unsort_list.at(i).second.y--;
				}
				else {
					break;
				}
			}
		}
	}
}
