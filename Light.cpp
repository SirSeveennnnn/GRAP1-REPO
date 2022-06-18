#include "Light.h"

glm::vec3 Light::GetLightPosition()
{
    return this->lightPos;
}

glm::vec3 Light::GetLightColor()
{
    return this->lightColor;
}

void Light::SetLightPosition(glm::vec3 Position)
{
    this->lightPos = Position;
}

void Light::SetLightColor(float r, float g, float b)
{
    lightColor = glm::vec3(r, g, b);
    ambientColor = glm::vec3(r, g, b);
}

void Light::SetLightBrightness(float intensity)
{
    this->brightness = intensity;
}

void Light::SetPointLight(bool value)
{
    this->pointLight = value;
}

void Light::SetDirectional(glm::vec3 directional)
{
    this->directional = directional;
}

void Light::ApplyLight(GLuint shader)
{
    GLuint lightAddress = glGetUniformLocation(shader, "lightPos");
    glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

    GLuint lightColorAddress = glGetUniformLocation(shader, "lightColor");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

    GLuint ambientColorAddress = glGetUniformLocation(shader, "ambientColor");
    glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

    GLuint ambientStrAddress = glGetUniformLocation(shader, "ambientStr");
    glUniform1f(ambientStrAddress, ambientStr);

    GLuint specStrAddress = glGetUniformLocation(shader, "specStr");
    glUniform1f(specStrAddress, specStr);

    GLuint specPhongAddress = glGetUniformLocation(shader, "specPhong");
    glUniform1f(specPhongAddress, specPhong);

    GLuint brightnessAddress = glGetUniformLocation(shader, "brightness");
    glUniform1f(brightnessAddress, brightness);

    GLuint pointLightAddress = glGetUniformLocation(shader, "pointLight");
    glUniform1i(pointLightAddress, pointLight);

    GLuint directionalAddress = glGetUniformLocation(shader, "directional");
    glUniform3fv(directionalAddress, 1, glm::value_ptr(directional));

}
