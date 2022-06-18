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

class Skybox
{
public:
	Skybox();
	void Draw(GLuint ShaderProgram, bool perspective, PerspectiveCamera perspectiveCamera, OrthoCamera orthoCamera, Light light);

private:
	GLuint skyboxVAO, skyboxVBO, skyboxEBO;
	unsigned int skyBoxTex;
};

