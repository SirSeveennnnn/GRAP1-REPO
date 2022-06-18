#include "PerspectiveCamera.h"



PerspectiveCamera::PerspectiveCamera():Camera()
{
	SetProjection(800, 800);
}

void PerspectiveCamera::SetProjection(float height, float width)
{
	this->perspectiveProjection = glm::perspective(glm::radians(90.0f), (height / width), 0.1f, 500.0f);
	
}

inline glm::mat4 PerspectiveCamera::GetPerspectiveProjection()
{
	return this->perspectiveProjection;
}

void PerspectiveCamera::ApplyProjection(GLuint ShaderProgram)
{
	unsigned int projLoc = glGetUniformLocation(ShaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(perspectiveProjection));
}

void PerspectiveCamera::Orbital(float X, float Y, float deltaTime)
{
	/*
	glm::vec2 input = glm::normalize(glm::vec2(X - 300, Y - 300));
	cameraOrientation = glm::rotate(cameraOrientation, glm::radians(20.f * deltaTime), glm::vec3(-input.y, input.x, 0));
	viewMatrix = cameraOrientation * cameraPositionMatrix;
	*/
}

void PerspectiveCamera::UpdateViewMatrix(glm::mat4 targetTransform)
{
	glm::vec3 target;
	target.x = targetTransform[3][0];
	target.y = targetTransform[3][1];
	target.z = targetTransform[3][2];


	F = glm::normalize(glm::vec3(cameraPos- target));
	R = glm::normalize(glm::cross(worldUp, F));
	U = glm::cross(F, R);

	viewMatrix = glm::lookAt(cameraPos, target + F, worldUp);
}

void PerspectiveCamera::FollowTarget(glm::mat4 targetTransform, unordered_map<string,bool>input, glm::vec2 mouse)
{
	glm::vec3 targetPosition;
	targetPosition.x = targetTransform[3][0];
	targetPosition.y = targetTransform[3][1];
	targetPosition.z = targetTransform[3][2];

	glm::vec3 direction;

	direction.x = cos(glm::radians(mouse.x)) * cos(glm::radians(mouse.y));
	direction.y = sin(glm::radians(mouse.y));
	direction.z = sin(glm::radians(mouse.x)) * cos(glm::radians(mouse.y));
	direction = glm::normalize(direction);
	offset = (direction * 15.f) + targetPosition;

	cameraPos.x = offset.x;
	cameraPos.y = offset.y;
	cameraPos.z = offset.z;
	
	cameraPos.x = offset.x;
	cameraPos.y = offset.y;
	cameraPos.z = offset.z;
	
}
