#pragma once

#include"Model.h"

using namespace std;

class Planet

{
public:
    Planet(string path);
    void SetupTexture(string path);
    void Draw(GLuint ShaderProgram, bool perspective, PerspectiveCamera perspectiveCamera, OrthoCamera orthoCamera);
    void DeleteBuffers();
   
    glm::mat4 GetTransform();
    glm::mat4 GetPivot();

private:
    GLuint texture;
    GLuint normalMap;

    std::string Path = "3D/Planet.obj";
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

    float TURN_SPEED = 150.f;

};

