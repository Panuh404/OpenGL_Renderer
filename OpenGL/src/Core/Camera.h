#pragma once
#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

// Define Several possible options for the camera movement
enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
constexpr float YAW			= -90.f;
constexpr float PITCH		= 0.0f;
constexpr float SPEED		= 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM		= 45.0f;

class Camera
{
public:
	// Constructor (Vectors)
	Camera(	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW, float pitch = PITCH);
	// Constructos (Scalars)
	Camera( float xPos, float yPos, float zPos,
			float xUp,	float yUp,	float zUp,
			float yaw,	float pitch);

	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yOffset, float MaxZoom = 45.0f);

	inline glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }

public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler Angles
	float Yaw;
	float Pitch;

	// Camera Options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

private:
	void UpdateCameraVectors();
};
