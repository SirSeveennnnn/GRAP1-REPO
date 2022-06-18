#pragma once
#include "Model.h"

class Debris
{
private:

    GLuint texture;

    std::string Path = "3D/spaceship.obj";
    std::vector<tinyobj::shape_t> Shapes;
    std::vector<tinyobj::material_t> Material;
    std::string warning, error;

    tinyobj::attrib_t Attributes;

    bool Success;

    std::vector<GLuint> indices;
    std::vector<GLfloat> fullVertexData;

    //Texutures
    int img_width, img_height, colorChannels;
    unsigned char* tex_bytes;

    //Buffers
    GLuint VAO, VBO;

    //Transform
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);
    glm::mat4 Transform;
    glm::mat4 pivot;

    //Variables
    float SPEED = 500.f;
    float VERTICAL_SPEED = 150.f;
    float TURN_SPEED = 150.f;

    float radius = 50.f;
    float rotation = 0;
    float axis = 0;

public:
    Debris(string path);
    void SetupTexture(string path);
    void Draw(GLuint ShaderProgram, bool perspective, PerspectiveCamera perspectiveCamera, OrthoCamera orthoCamera, Light light);
    void DeleteBuffers();
    void SetScale(glm::vec3 scale);
    void SetRadius(float radius);
    void Orbit(float deltaTime, float offset, float rotSpeed, float axisSpeed, float y);

};

