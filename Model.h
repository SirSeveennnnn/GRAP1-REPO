#pragma once

#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<string>
#include<vector>
#include<iostream>

#include"tiny_obj_loader.h"
#include"stb_image.h"

#include "PerspectiveCamera.h"
#include "OrthoCamera.h"
#include "Light.h"

#include<unordered_map>

using namespace std;

class Model
{
private:
    GLuint texture;
    GLuint normalMap;

    std::string Path = "3D/spaceship.obj";
    std::vector<tinyobj::shape_t> Shapes;
    std::vector<tinyobj::material_t> Material;
    std::string warning, error;

    tinyobj::attrib_t Attributes;

    bool Success;

    std::vector<GLuint> indices;
    std::vector<GLfloat> fullVertexData;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    //Texutures
    int img_width, img_height, colorChannels;
    unsigned char* tex_bytes;

    int norm_img_width, norm_img_height, norm_colorChannels;
    unsigned char* norm_tex_bytes;

    

    //Buffers
    GLuint VAO, VBO;

    //Transform
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);
    glm::mat4 Transform;
    glm::mat4 pivot;

    //Variables
    float SPEED = 800.f;
    float VERTICAL_SPEED = 400.f;
    float TURN_SPEED = 300.f;
    
public:

    Model(string path);
    void SetupTexture(string path, string path2);
    void Draw(GLuint ShaderProgram, bool perspective, PerspectiveCamera perspectiveCamera, OrthoCamera orthoCamera, Light light);
    void DeleteBuffers();
    void PlayerControls(unordered_map<string,bool> input, float deltaTime, PerspectiveCamera camera);

    glm::mat4 GetTransform();
    glm::mat4 GetPivot();
    GLuint GetTexture();
    
};

