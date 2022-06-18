#pragma once
#include "camera.h"
#include<unordered_map>


class OrthoCamera : public Camera
{
public:
	OrthoCamera();
	glm::mat4 GetOrthoProjection();
	void ApplyProjection(GLuint ShaderProgram);
	void TopView(unordered_map<string,bool>input, float deltaTime);
	

private:
	glm::mat4 orthoProjection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, -000.0f, 500.0f);

};

