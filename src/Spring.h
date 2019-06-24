#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>
#include "Entity.h"

class Spring{
public:
	Spring();
	void setEndPoint(Entity *obj1, Entity *obj2);
	void setCoefficient(float k, float d);
	Entity *end1;
	Entity *end2;
	float k;
	float d;
	float initial_length;
	bool enable = true;
};