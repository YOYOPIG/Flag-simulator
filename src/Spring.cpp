#include "Spring.h"

Spring::Spring(){
	
}

void Spring::setEndPoint(Entity *obj1, Entity *obj2){
	this->end1 = obj1;
	this->end2 = obj2;
	glm::vec3 point1 = this->end1->rigid.data.position;
	glm::vec3 point2 = this->end2->rigid.data.position;
	this->initial_length = glm::length(glm::vec3(point1 - point2));
	if (initial_length != 0.1 && initial_length - 1.414 > 0.001){
		printf("wrong length\n");
	}
}

void Spring::setCoefficient(float k, float d){
	this->k = k;
	this->d = d;
}