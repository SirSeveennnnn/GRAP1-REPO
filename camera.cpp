#include "camera.h"

Camera::Camera()
{
}

glm::vec3 Camera::GetCameraPosition()
{
	return this->cameraPos;
}

glm::mat4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

glm::vec3 Camera::GetFront()
{
	return this->F;
}

void Camera::ApplyCameraPosition(GLuint ShaderProgram)
{
	GLuint cameraPosAddress = glGetUniformLocation(ShaderProgram, "cameraPos");
	glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));
}

void Camera::ApplyView(GLuint ShaderProgram)
{
	unsigned int viewLoc = glGetUniformLocation(ShaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}
