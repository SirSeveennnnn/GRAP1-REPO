#pragma once

#include "Model.h"

using namespace std;

class MySkybox
{
public:
	MySkybox();
	void Draw(GLuint ShaderProgram, bool perspective, PerspectiveCamera perspectiveCamera, OrthoCamera orthoCamera);
	void DeleteBuffers();

private:
	GLuint skyboxVAO, skyboxVBO, skyboxEBO;
	unsigned int skyBoxTex;
};

