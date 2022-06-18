#include "OrthoCamera.h"

OrthoCamera::OrthoCamera()
{
    cameraPos = glm::vec3(0.0f, 100.0f, 0.0f);
    Center = glm::vec3(0.0f, 0.0f, 0.0f);
    F = glm::normalize(cameraPos - Center);

    worldUp = glm::vec3(0.0f, 0.0f, -1.0f);
    R = glm::normalize(glm::cross(worldUp, F));
    U = glm::cross(F, R);

    viewMatrix = glm::lookAt(cameraPos, F, worldUp);
}

glm::mat4 OrthoCamera::GetOrthoProjection()
{
    return this->orthoProjection;
}

void OrthoCamera::ApplyProjection(GLuint ShaderProgram)
{
    unsigned int projLoc = glGetUniformLocation(ShaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(orthoProjection));
}

void OrthoCamera::TopView(unordered_map<string, bool> input, float deltaTime)
{
    
   
    float SPEED = 100.f;

    if (input["W"] == true)
    {
        cameraPos.z -= SPEED * deltaTime;
    }

    if (input["S"] == true)
    {
        cameraPos.z += SPEED * deltaTime;
    }

    if (input["A"] == true)
    {
        cameraPos.x -= SPEED * deltaTime;
    }

    if (input["D"] == true)
    {
        cameraPos.x += SPEED * deltaTime;
    }
     
    F = glm::vec3(0, -1, 0);
    viewMatrix = glm::lookAt(cameraPos, cameraPos + F, worldUp);
   
}
