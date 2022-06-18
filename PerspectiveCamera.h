#pragma once
#include "camera.h"
#include<unordered_map>

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera();

	void SetProjection(float height, float width);
	glm::mat4 GetPerspectiveProjection();

	void ApplyProjection(GLuint ShaderProgram);

	void Orbital(float X, float Y, float deltaTime);
	void UpdateViewMatrix(glm::mat4 targetTransform);
	void FollowTarget(glm::mat4 targetTransform, unordered_map<string, bool>input, glm::vec2 mouse);
	glm::mat4 perspectiveProjection;

private:

	glm::vec3 offset;
	glm::mat4 pivot = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	float rotationSpeed = 100.f;

};


