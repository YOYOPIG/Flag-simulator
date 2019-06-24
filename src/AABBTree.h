#pragma once
#include "Entity.h"

AABB unionAABB(AABB ab1, AABB ab2);
float shortestDistance(AABB ab1, AABB ab2);

class AABBNode {
public:
	AABBNode *_parent;
	AABBNode *_children[2];


	//node's bounding box
	AABB _aabb;
	//things that should be collide
	Entity *_data;
	AABBNode();
	bool isLeaf();
	void setBranch(AABBNode *n1, AABBNode *n2);
	void setLeaf(Entity * e);
	void updateAABB(float margin);
	//sibling : 兄弟
	AABBNode *getSibling();

};

class AABBTree {
public:
	AABBTree();
	void setMargin(float m);
	void add(Entity *e);
	void remove(AABBNode *node);
	void update();
	std::vector<Entity *> &collisionWith(AABBNode *);

	AABBNode *_root;

	//margin 大小影響偵測到的碰撞體數量及更新頻率
	float margin;

private:
	void insert(AABBNode *node, AABBNode **parent);
	void findInvalidNode(AABBNode *node);
	void collidingHelperDown(AABBNode *node, AABBNode *cmp);
	void collidingHelperUp(AABBNode *node, AABBNode *parent);
	bool collidingHelper(AABBNode *node, AABBNode *sibling);
	std::vector<AABBNode *> invalid_node;
	std::vector<Entity *> collision_list;
};