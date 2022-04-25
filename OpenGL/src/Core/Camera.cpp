#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	Front(glm::vec3(0.0f,0.0f,-1.0f)),
	MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

Camera::Camera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp, float yaw, float pitch) :
	Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(xPos, yPos, zPos);
	WorldUp = glm::vec3(xUp, yUp, zUp);
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	const float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)	Position += Front * velocity;
	if (direction == BACKWARD)	Position -= Front * velocity;
	if (direction == LEFT)		Position -= Right * velocity;
	if (direction == RIGHT)		Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;
	Yaw		+= xOffset;
	Pitch	+= yOffset;

	// Make Sure that when pitch is out of bounds, screen doesn't get flipped
	if(constrainPitch)
	{
		if (Pitch >  89.0f) Pitch =  89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
	}
	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset, float MaxZoom)
{
	Zoom -= (float)yOffset;
	// Zoom Limit;
	if (Zoom <  1.0f) Zoom =  1.0f;
	if (Zoom > MaxZoom) Zoom = MaxZoom;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front; // Calculate new front
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up	  = glm::normalize(glm::cross(Right, Front));
}