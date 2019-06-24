#include "Entity.h"
#include <iostream>
using namespace std;

#define CONTAIN_LIMIT 0.1

AABB::AABB()
{
	_left_down_back = glm::vec3(0.0f, 0.0f, 0.0f);
	_right_top_front = glm::vec3(0.0f, 0.0f, 0.0f);
}

void AABB::unionAABB(AABB ab1)
{
	float left, right, top, down, front, back;
	left = ab1._left_down_back.x > _left_down_back.x ? _left_down_back.x : ab1._left_down_back.x;
	down = ab1._left_down_back.y > _left_down_back.y ? _left_down_back.y : ab1._left_down_back.y;
	right = ab1._right_top_front.x > _right_top_front.x ? ab1._right_top_front.x : _right_top_front.x;
	top = ab1._right_top_front.y > _right_top_front.y ? ab1._right_top_front.y : _right_top_front.y;
	front = ab1._right_top_front.z > _right_top_front.z ? ab1._right_top_front.z : _right_top_front.z;
	back = ab1._right_top_front.z < _right_top_front.z ? ab1._right_top_front.z : _right_top_front.z;
	_left_down_back.x = left;
	_left_down_back.y = down;
	_right_top_front.x = right;
	_right_top_front.y = top;
	_right_top_front.z = front;
	_left_down_back.z = back;
}

bool AABB::contain(AABB ab1)
{//檢查此 AABB 有無包含參數 ab1
	if (ab1._left_down_back.x < _left_down_back.x - CONTAIN_LIMIT) {
		return false;
	}
	if (ab1._left_down_back.y < _left_down_back.y - CONTAIN_LIMIT) {
		return false;
	}
	if (ab1._left_down_back.z < _left_down_back.z - CONTAIN_LIMIT) {
		return false;
	}
	if (ab1._right_top_front.x > _right_top_front.x + CONTAIN_LIMIT) {
		return false;
	}
	if (ab1._right_top_front.y > _right_top_front.y + CONTAIN_LIMIT) {
		return false;
	}
	if (ab1._right_top_front.z > _right_top_front.z + CONTAIN_LIMIT) {
		return false;
	}
	return true;
}

bool AABB::isCollided(AABB ab1)
{
	if ((ab1._left_down_back.x<_right_top_front.x && ab1._right_top_front.x>_left_down_back.x) || (ab1._left_down_back.x>_right_top_front.x && ab1._right_top_front.x<_left_down_back.x)) {
		if ((ab1._left_down_back.y<_right_top_front.y && ab1._right_top_front.y>_left_down_back.y) || (ab1._left_down_back.y>_right_top_front.y && ab1._right_top_front.y<_left_down_back.y)) {
			if ((ab1._left_down_back.z<_right_top_front.z && ab1._right_top_front.z>_left_down_back.z) || (ab1._left_down_back.z>_right_top_front.z && ab1._right_top_front.z<_left_down_back.z)) {
				return true;
			}
		}
	}
	return false;
}

float AABB::volume()
{
	return (_right_top_front.x - _left_down_back.x)*(_right_top_front.y - _left_down_back.y)*(_right_top_front.z - _left_down_back.z);
}


Entity::Entity(int size)
{
	vertex_size = size;
	_id = createID();
	e_type = NORMAL;
	_trigger = false;
	_visible = true;
}


Entity::~Entity()
{
}

void Entity::setSize(int s)
{
	vertex_size = s;
}

int Entity::size()
{
	return vertex_size;
}

bool Entity::isVisible()
{
	return _visible;
}

void Entity::setTrigger(bool value)
{
	_trigger = value;
}

bool Entity::isTrigger()
{
	return _trigger;
}

void Entity::trigger()
{
}

int Entity::_obj_num = 0;

int Entity::createID()
{
	_obj_num++;
	return _obj_num;
}



RigidBody::RigidBody(RigidType type)
{
	this->type = type;
	this->_mass = 1;
}


void RigidBody::setRadius(float r)
{
	_radius = r;
}

void RigidBody::setPosition(float x, float y, float z)
{//在此重設AABB
	data.position.x = x;
	data.position.y = y;
	data.position.z = z;
	setAABB();
}

void RigidBody::changePosition(float x, float y, float z)
{
	data.position.x += x;
	data.position.y += y;
	data.position.z += z;
	setAABB();
}

void RigidBody::setPosition(glm::vec3 pos)
{
	data.position.x = pos.x;
	data.position.y = pos.y;
	data.position.z = pos.z;
	setAABB();
}

void RigidBody::changePosition(glm::vec3 changePos)
{
	data.position.x += changePos.x;
	data.position.y += changePos.y;
	data.position.z += changePos.z;
	setAABB();
}

void RigidBody::setAABB()
{
	if (type == SPHERE) {
		bounding_box._left_down_back.x = data.position.x - _radius-0.005;
		bounding_box._left_down_back.y = data.position.y - _radius-0.005;
		bounding_box._right_top_front.x = data.position.x + _radius+0.005;
		bounding_box._right_top_front.y = data.position.y + _radius+0.005;
		bounding_box._left_down_back.z = data.position.z - _radius-0.005;
		bounding_box._right_top_front.z = data.position.z + _radius+0.005;
	}

}

AABB * RigidBody::getAABB()
{
	return &bounding_box;
}


float RigidBody::getRadius()
{
	return _radius;
}


SphereObject::SphereObject(int size) : Entity(size)
{
}

GroundObject::GroundObject(int size) : Entity(size)
{
}

Player::Player(int size) : Entity(size)
{
}




