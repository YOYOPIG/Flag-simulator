#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Entity.h"
#include "Spring.h"
#include "ObjectManager.h"

class Physics
{
public:
	//all the function should be static
	static void update(Entity *obj, float dt, Entity **list);
	static int collisionDetect(Entity *A, Entity *B);
	//this function is using to do View Frustum Culling 
	static bool GJK(std::vector<glm::vec3> vertices1, std::vector<glm::vec3> vertices2, float radius = 0);
	static glm::vec3 GJKSupport(std::vector<glm::vec3> vertices, std::vector<bool> &has_find, glm::vec3 dir, bool &end);
	static bool GJKSupportRect(std::vector<glm::vec3> vertices, std::vector<bool> has_find, glm::vec3 dir, bool &end);
	//fast inverse square root
	//please don't put integer into this function
	static float qsqrt(float number);
	Physics();
	~Physics();
	
	static void calcSpring(Spring * spring, float dt);
	static void wind(Entity *obj, float dt, glm::vec3 direction, float force);
private:
	const static float g;//重力加速度
	static void displace(Entity *obj, float dt);
	static void force(Entity *obj, glm::vec3 f, float dt);
	static void force(Entity *obj, float x, float y, float z, float dt);
	static void resistance(Entity *obj, float dt); //阻力
};

