#ifndef CAMERA_H
#define CAMERA_H
#pragma once
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <ctime>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SPACE,
	SHIFT
};

// Default camera values
const float YAW = -90.0;
const float PITCH = 0.0;
const float SPEED = 10;
const float SENSITIVITY = 0.08;
const float ZOOM = 45.0;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL

class TPCamera
{
public:
	// Camera Attributes
	//	int count = 0;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;
	glm::vec3 Position;
	glm::vec3 _v;
	// Eular Angles
	float yaw;
	float pitch;
	float speed;
	float sensitivity;
	float zoom;
	float radius = 3;
	bool _is_moving = false;
	bool _is_jumping = false;
	int _jump_cool_down;

	TPCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float y = YAW, float p = PITCH) : Front(0.0f, 0.0f, -1.0f), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM) {
		Position = position;
		_v = glm::vec3(0.0f, 0.0f, 0.0f);
		WorldUp = up;
		yaw = y+90;
		pitch = p;
		updateCamera();
	}
	TPCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float y = YAW, float p = PITCH) : Front(0.0f, 0.0f, -1.0f), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM) {
		Position.x = posX;
		Position.y = posY;
		Position.z = posZ;
		WorldUp.x = upX;
		WorldUp.y = upY;
		WorldUp.z = upZ;
		_v = glm::vec3(0.0f, 0.0f, 0.0f);
		yaw = y;
		pitch = p;
		updateCamera();
	}
	~TPCamera() {}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 getViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(CameraMovement direction, float delta_time) {
		float velocity = speed*delta_time;
		_is_moving = true;
		if (direction == FORWARD) {
			//Position.x += Front.x*velocity;
			//Position.z += Front.z*velocity;
			if (glm::length(_v) < 20) {
				_v += glm::vec3(Front.x*velocity*16, 0.0f, Front.z*velocity*16);
			}
			else {
				_v *= 0.5;
				_v += glm::vec3(Front.x*velocity * 12, 0.0f, Front.z*velocity * 12);
			}
		}
		if (direction == BACKWARD) {
			//Position.x -= Front.x*velocity;
			//Position.z -= Front.z*velocity;
			if (glm::length(_v) < 20) {
				_v += glm::vec3(-Front.x*velocity*16, 0.0f, -Front.z*velocity*16);
			}
			else {
				_v *= 0.5;
				_v += glm::vec3(-Front.x*velocity * 12, 0.0f, -Front.z*velocity * 12);
			}
		}
		if (direction == LEFT) {
			//Position.x -= Right.x*velocity;
			//Position.z -= Right.z*velocity;
			if (glm::length(_v) < 20) {
				_v += glm::vec3(-Right.x*velocity*16, 0.0f, -Right.z*velocity*16);
			}
			else {
				_v *= 0.5;
				_v += glm::vec3(-Right.x*velocity * 12, 0.0f, -Right.z*velocity * 12);
			}
		}
		if (direction == RIGHT) {
			//Position.x += Right.x*velocity;
			//Position.z += Right.z*velocity;
			if (glm::length(_v) < 20) {
				_v += glm::vec3(Right.x*velocity*16, 0.0f, Right.z*velocity*16);
			}
			else {
				_v *= 0.5;
				_v += glm::vec3(Right.x*velocity * 12, 0.0f, Right.z*velocity * 12);
			}
		}
		if (direction == SPACE) {
			if (!_is_jumping) {
				_v += glm::vec3(0.0f, 5.0f, 0.0f);
			std::cerr << "space!!" << std::endl;
				_jump_cool_down = clock();
				_is_jumping = true;
			}
			if (clock() - _jump_cool_down > 300) {
				_is_jumping = false;
			}
		}
		if (direction == SHIFT) {
			if (!_is_jumping) {
				_v += glm::vec3(0.0f, -5.0f, 0.0f);
				std::cerr << "shift!!" << std::endl;
				_jump_cool_down = clock();
				_is_jumping = true;
			}
			if (clock() - _jump_cool_down > 300) {
				_is_jumping = false;
			}
		}
		//		std::cerr << "press keyboard" <<count<< std::endl;
		//		++count;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseEvent(float xoffset, float yoffset, bool constrain_pitch = true) {
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89) {
			yoffset = yoffset - pitch + 89;
		}
		if (pitch < -89) {
			yoffset = yoffset - pitch - 89;
		}
		//先回到 world coordinate 旋轉再轉回 camera coordinate
		glm::vec3 basis = -(Position + radius * Front);
		glm::mat4 trans1, trans2, trans3;
		trans1 = glm::translate(trans1, basis);
		trans2 = glm::rotate(trans2, glm::radians(-xoffset), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 front2d = glm::vec3(Front.x, 0.0f, Front.z);		
		glm::vec3 rotate_axis = glm::cross(front2d, glm::vec3(0.0f, 1.0f, 0.0f));//上下旋轉的旋轉軸
		trans2 = glm::rotate(trans2, glm::radians(yoffset), rotate_axis);
		trans3 = glm::translate(trans3, -basis);
		Position = glm::vec3(trans3 * trans2 * trans1 * glm::vec4(Position, 1.0f));
		
		
		

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrain_pitch) {
			if (pitch > 89) {
				pitch = 89;
			}
			if (pitch < -89) {
				pitch = -89;
			}
		}
		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCamera();
	}

	float processScrollEvent(float yoffset) {
		yoffset *= 5;
		if (zoom >= 10.0f && zoom <= 45.0f)
			zoom -= yoffset;
		if (zoom <= 10.0f)
			zoom = 10.0f;
		if (zoom >= 45.0f)
			zoom = 45.0f;
		return zoom;
	}

private:
	void updateCamera() {
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));

	}
};



class Camera2D
{
public:
	// Camera Attributes
	//	int count = 0;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;
	glm::vec3 Position;
	glm::vec3 _v;
	// Eular Angles

	float speed;
	float sensitivity;
	float zoom;
	float radius = 3;
	bool _is_moving = false;
	bool _is_jumping = false;
	int _jump_cool_down;

	Camera2D(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 0.0f, -1.0f)) : Front(0.0f, -3.0f, 0.0f), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM) {
		Position = position;
		Position.y += 3;
		_v = glm::vec3(0.0f, 0.0f, 0.0f);
		WorldUp = glm::vec3(0.0f, 0.0f, 1.0f);
		updateCamera();
	}
	Camera2D(float posX, float posY, float posZ, float upX, float upY, float upZ) : Front(0.0f, -3.0f, 0.0f), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM) {
		Position.x = posX;
		Position.y = posY+3;
		Position.z = posZ;
		WorldUp.x = 0;
		WorldUp.y = 0;
		WorldUp.z = -1;
		_v = glm::vec3(0.0f, 0.0f, 0.0f);
		updateCamera();
	}
	~Camera2D() {}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 getViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(CameraMovement direction, float delta_time) {
		float velocity = speed*delta_time;
		_is_moving = true;
		if (direction == FORWARD) {
			//Position.x += Front.x*velocity;
			//Position.z += Front.z*velocity;
			if (glm::length(_v) < 8) {
				_v += glm::vec3(Front.x*velocity * 6, 0.0f, Front.z*velocity * 6);
			}
			else {
				_v *= 0.5;
				_v += glm::vec3(Front.x*velocity * 5, 0.0f, Front.z*velocity * 5);
			}
		}
		if (direction == BACKWARD) {
			//Position.x -= Front.x*velocity;
			//Position.z -= Front.z*velocity;
			if (glm::length(_v) < 8) {
				_v += glm::vec3(-Front.x*velocity * 6, 0.0f, -Front.z*velocity * 6);
			}
			else {
				_v *= 0.5;
				_v += glm::vec3(-Front.x*velocity * 5, 0.0f, -Front.z*velocity * 5);
			}
		}
		if (direction == LEFT) {
			//Position.x -= Right.x*velocity;
			//Position.z -= Right.z*velocity;
			if (glm::length(_v) < 8) {
				_v += glm::vec3(-Right.x*velocity * 6, 0.0f, -Right.z*velocity * 6);
			}
			else {
				_v *= 0.5;
				_v += glm::vec3(-Right.x*velocity * 5, 0.0f, -Right.z*velocity * 5);
			}
		}
		if (direction == RIGHT) {
			//Position.x += Right.x*velocity;
			//Position.z += Right.z*velocity;
			if (glm::length(_v) < 8) {
				_v += glm::vec3(Right.x*velocity * 6, 0.0f, Right.z*velocity * 6);
			}
			else {
				_v *= 0.5;
				_v += glm::vec3(Right.x*velocity * 5, 0.0f, Right.z*velocity * 5);
			}
		}
		if (direction == SPACE) {

		}
		//		std::cerr << "press keyboard" <<count<< std::endl;
		//		++count;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseEvent(float xoffset, float yoffset, bool constrain_pitch = true) {
		xoffset *= sensitivity;
		yoffset *= sensitivity;




		// Make sure that when pitch is out of bounds, screen doesn't get flipped

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCamera();
	}

	float processScrollEvent(float yoffset) {
		yoffset *= 5;
		if (zoom >= 10.0f && zoom <= 45.0f)
			zoom -= yoffset;
		if (zoom <= 10.0f)
			zoom = 10.0f;
		if (zoom >= 45.0f)
			zoom = 45.0f;
		return zoom;
	}

private:
	void updateCamera() {
		// Calculate the new Front vector
		//glm::vec3 front;
		//Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));

	}
};


#endif