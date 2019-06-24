#include "Physics.h"
#include <iostream>
#include <stdlib.h>

#define MAX_DT 0.016
#define MIN_DT 0.001
#define TERMINAL_SPEED 150
#define ALLOW_DIST 0.001
const float Physics::g = 0.098;

void Physics::update(Entity * obj,float dt, Entity **list)
{
	if (dt > MAX_DT) {
		dt = MAX_DT;
	}
	if (dt < MIN_DT) {
		dt = MIN_DT;
	}

	if (obj->e_type == PLAYER) {
	//	cout << obj->rigid.data.position.x << " " << obj->rigid.data.position.y << " " << obj->rigid.data.position.z << endl;
	}

	if (!obj->rigid._is_static) {
		//Force
		if ((obj->_id - 1) % 20 == 0){
			obj->rigid.data.velocity = glm::vec3();
			return;
		}
		if (obj->rigid.data.velocity.y > -TERMINAL_SPEED) {//test before ball with surface collision done
			float rnd = rand() % 100 * 0.00001;
			force(obj, 0, -obj->rigid._mass * g, 0, dt);
		}
		//Force
		//Displace
		displace(obj, dt);
		//Displace
		

		//Collision
		if (obj->rigid.type != NO_COLLIDE) {
			
			for (int compute_limit = 0; compute_limit < 10; ++compute_limit){
				//find collision pair
				std::vector<Entity *> collide_list = ObjectManager::aabb_tree.collisionWith(obj->_aabb_node);
				//std::vector<Entity *> collide_list = ObjectManager::object_list;
				//if(obj->e_type==PLAYER)
				//	cout << collide_list.size() << endl;
				if (collide_list.size() == 0){
					break;
				}
				for (int i = 0; i < collide_list.size(); ++i) {
					if (collide_list.at(i)->_id == obj->_id) {
						continue;
					}

					int is_collided = collisionDetect(obj, collide_list.at(i));

					if (is_collided == -1) {is_collided = 1;}
					if (is_collided == 1) {
						//球碰撞反應
						if (obj->rigid.type == SPHERE &&collide_list.at(i)->rigid.type == SPHERE) {
							float dx = (obj->rigid.data.position.x - collide_list.at(i)->rigid.data.position.x);
							float dy = (obj->rigid.data.position.y - collide_list.at(i)->rigid.data.position.y);
							float dz = (obj->rigid.data.position.z - collide_list.at(i)->rigid.data.position.z);
							float restrict = (obj->rigid._restitution_coeffient + collide_list.at(i)->rigid._restitution_coeffient) / 2;
							glm::vec3 normal = glm::normalize(glm::vec3(dx, dy, dz));//B指向A
							//normal.z *= -1;//左手坐標系
							//normal.x *= -1;

							glm::vec3 relate_v = obj->rigid.data.velocity - collide_list.at(i)->rigid.data.velocity;
							float F;
							F = (-(1 + restrict)*(glm::dot(normal, relate_v))) / (((1 / obj->rigid._mass + 1 / collide_list.at(i)->rigid._mass)));//其實是衝量

							force(obj, F * normal, dt);
							force(collide_list.at(i), -F * normal, dt);
						}
						//球碰撞反應
					}
				}//*/
				displace(obj, dt);
				resistance(obj, dt);
			}
		}
		//Collision
	}

}



int Physics::collisionDetect(Entity * A, Entity * B)
{
	//球對球碰撞偵測
	if (A->rigid.type == SPHERE&&B->rigid.type == SPHERE) {
		float dx = abs(A->rigid.data.position.x - B->rigid.data.position.x);
		float dy = abs(A->rigid.data.position.y - B->rigid.data.position.y);
		float dz = abs(A->rigid.data.position.z - B->rigid.data.position.z);
		float dist = (dx*dx + dy*dy + dz*dz);//平方
		//cout <<	dist << endl;
		if (dist < pow(A->rigid.getRadius() + B->rigid.getRadius() - ALLOW_DIST, 2)) {
			return -1;
		}
		else if (dist <= pow(A->rigid.getRadius() + B->rigid.getRadius(), 2)) {
			return 1;
		}
		else {
			return 0;
		}
	}
	//


	return 0;//無碰撞物體回傳值
}


bool Physics::GJK(std::vector<glm::vec3> vertices1, std::vector<glm::vec3> vertices2, float radius)
{
	std::vector<glm::vec3> point_set;
	for (int i = 0; i < vertices1.size(); ++i) {
		for (int j = 0; j < vertices2.size(); ++j) {
			point_set.push_back(vertices1.at(i) - vertices2.at(j));
		}
	}
	std::vector<bool> has_find;
	has_find.resize(point_set.size());
	has_find.at(0) = true;

	//傳進 support 檢查是否相交
	bool end = false;

	glm::vec3 first_point = point_set.at(0);
	glm::vec3 dir2origin = -first_point;
	while (1) {
		glm::vec3 second_point = GJKSupport(point_set, has_find, dir2origin, end);
		if (end) {
			return end;
		}
		if (glm::dot(dir2origin, second_point) < 0) {
			return false;
		}
		float p = glm::dot(dir2origin, second_point - first_point);
		dir2origin = -(first_point + p*(second_point - first_point));
		first_point = second_point;
	}
	return false;
}

glm::vec3 Physics::GJKSupport(std::vector<glm::vec3> vertices, std::vector<bool> &has_find, glm::vec3 dir, bool &end)
{
	//檢查所有點和至原點向量的內積並回傳內積結果最大的點
	//若最大點已找過代表包含原點，設 end 為 true
	float dot_result = -100;
	int result = 0;
	for (int i = 0; i < vertices.size(); ++i){
		float tmp = glm::dot(vertices.at(i), dir);
		if (tmp > dot_result) {
			result = i;
			dot_result = tmp;
		}
	}
	if (has_find.at(result)) {
		end = true;
	}
	else {
		has_find.at(result) = true;
	}
	if (vertices.at(result) == glm::vec3(0.0f, 0.0f, 0.0f)) {
		end = true;
	}
	return vertices.at(result);
}

float Physics::qsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;                       // evil floating point bit level hacking（对浮点数的邪恶位元hack）
	i = 0x5f3759df - (i >> 1);               // what the fuck?（这他妈的是怎么回事？）
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration （第一次迭代）
										   //      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed（第二次迭代，可以删除）

	return y;
}

void Physics::displace(Entity *obj, float dt){
	glm::vec3 dis = obj->rigid.data.velocity * dt;
	if (glm::length(dis) < 0.000001) {
		dis.x = 0;
		dis.y = 0;
		dis.z = 0;
	}
	obj->rigid.data.position += dis;
}

void Physics::force(Entity *obj, glm::vec3 f, float dt){
	obj->rigid.data.velocity += f * dt / obj->rigid._mass;
}

void Physics::force(Entity *obj, float x, float y, float z, float dt){
	obj->rigid.data.velocity += glm::vec3(x, y, z) * dt / obj->rigid._mass;
}

void Physics::resistance(Entity *obj, float dt){
	obj->rigid.data.velocity -= obj->rigid.data.velocity *0.05f * dt;
}

void Physics::calcSpring(Spring * spring, float dt){
	glm::vec3 point1 = spring->end1->rigid.data.position;
	glm::vec3 point2 = spring->end2->rigid.data.position;
	glm::vec3 v1 = spring->end1->rigid.data.velocity;
	glm::vec3 v2 = spring->end2->rigid.data.velocity;
	glm::vec3 distance = glm::vec3(point1 - point2);
	float length = glm::length(distance);
	if (length == 0){
		length = 0.00001;
	}
	if (spring->end1->_id == 301 || spring->end1->_id == 302){
		return;
	}
	
	float k_force = spring->k * (length - spring->initial_length);
	float d_force = spring->d * glm::dot((v1 - v2), distance) / length;
	
	glm::vec3 F = -(k_force + d_force) * distance / length;
	force(spring->end1, F, dt);
	force(spring->end2, -F, dt);
	

}

void Physics::wind(Entity *obj, float dt, glm::vec3 direction, float f){
	force(obj, f * direction, dt);
}

Physics::Physics()
{
}


Physics::~Physics()
{
}
