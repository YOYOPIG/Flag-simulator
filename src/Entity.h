#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
//#include "Render.h"
#include <string>
//#include "Event.h"

extern class AABBNode;

class AABB {//等軸包圍盒
public:
	glm::vec3 _left_down_back;
	glm::vec3 _right_top_front;

	AABB();
	void unionAABB(AABB ab1);
	bool contain(AABB ab1);
	bool isCollided(AABB ab1);
	float volume();
};

enum RigidType {
	NO_COLLIDE = 0,		//無碰撞
	TRIANGLE,	//三角形
	RECTANGLE,	//矩形			//僅作為靜態物體使用
	CIRCLE,		//圓形
	GROUND,		//網格
	CUBE,		//立方體
	SPHERE,		//球
	CYLINDER,	//圓柱
	LINE		//線
};

enum EntityType {
	NORMAL,
	PLAYER,
};

class DynamicData {
public:
	//速度
	glm::vec3 velocity;
	//位置
	glm::vec3 position;

};

//如果超出 AABB 樹的容許值則重新插入




class RigidBody {
public:
	RigidBody(RigidType type = RECTANGLE);

	void setPosition(float x, float y, float z);
	void changePosition(float x, float y, float z);
	void setPosition(glm::vec3 pos);
	void changePosition(glm::vec3 changePos);

	//圓形、球體半徑
	void setRadius(float r);
	float getRadius();

	void setAABB();
	AABB *getAABB();

	DynamicData data;


	//允許物理效果
	bool _allow_physics = true;
	//能不能移動
	bool _is_static = false;
	//質量
	float _mass = 1;
	//恢復係數，0為完全非彈碰，1為彈碰
	float _restitution_coeffient = 1;
	//摩擦係數
	float _friction_coeffient = 3;
	RigidType type;
private:
	void setNormal();

	float _radius;
	AABB bounding_box;
	
};


class Entity
{
public:
	Entity(int size = 0);
	~Entity();

	//碰撞箱
	RigidBody rigid;
	//繪圖相關 key
	std::string VAO;
	std::string VBO;
	std::string EBO;
	std::string texture;
	std::string specular;
	std::string shader;

	//set vertex size
	void setSize(int);
	int size();

	int _id;
	EntityType e_type;

	AABBNode *_aabb_node = nullptr;

	bool isVisible();
	void setTrigger(bool value);
	bool isTrigger();
	virtual void trigger();
private:
	int vertex_size;	//繪製時會用到(頂點數量)
	bool _trigger;
	bool _visible;
	static int createID();
	static int _obj_num;

	
};

class Player :public Entity {
public:
	Player(int size = 0);
};

class SphereObject :public Entity {
public:
	SphereObject(int size = 0);
};

class GroundObject :public Entity {
public:
	GroundObject(int size = 0);
};

