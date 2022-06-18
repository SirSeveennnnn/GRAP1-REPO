#pragma once
#include "Model.h"


Model::Model(string path)
{
    Path = path;

    Success = tinyobj::LoadObj(&Attributes, &Shapes, &Material, &warning, &error, Path.c_str());

    for (int i = 0; i < Shapes[0].mesh.indices.size(); i++)
    {
        indices.push_back(Shapes[0].mesh.indices[i].vertex_index);
    }

    for (int i = 0; i < Shapes[0].mesh.indices.size(); i += 3)
    {
        tinyobj::index_t vData1 = Shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = Shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = Shapes[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(Attributes.vertices[vData1.vertex_index * 3], Attributes.vertices[(vData1.vertex_index * 3) + 1], Attributes.vertices[(vData1.vertex_index * 3) + 2]);
        glm::vec3 v2 = glm::vec3(Attributes.vertices[vData2.vertex_index * 3], Attributes.vertices[(vData2.vertex_index * 3) + 1], Attributes.vertices[(vData2.vertex_index * 3) + 2]);
        glm::vec3 v3 = glm::vec3(Attributes.vertices[vData3.vertex_index * 3], Attributes.vertices[(vData3.vertex_index * 3) + 1], Attributes.vertices[(vData3.vertex_index * 3) + 2]);

        glm::vec2 uv1 = glm::vec2(Attributes.texcoords[(vData1.texcoord_index * 2)], Attributes.texcoords[(vData1.texcoord_index * 2) + 1]);
        glm::vec2 uv2 = glm::vec2(Attributes.texcoords[(vData2.texcoord_index * 2)], Attributes.texcoords[(vData2.texcoord_index * 2) + 1]);
        glm::vec2 uv3 = glm::vec2(Attributes.texcoords[(vData3.texcoord_index * 2)], Attributes.texcoords[(vData3.texcoord_index * 2) + 1]);

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);

    }

    
    for (int i = 0; i < Shapes[0].mesh.indices.size(); i++)
    {
        tinyobj::index_t vData = Shapes[0].mesh.indices[i];

        fullVertexData.push_back(Attributes.vertices[(vData.vertex_index * 3)]);
        fullVertexData.push_back(Attributes.vertices[(vData.vertex_index * 3) + 1]);
        fullVertexData.push_back(Attributes.vertices[(vData.vertex_index * 3) + 2]);

        fullVertexData.push_back(Attributes.normals[(vData.normal_index * 3)]);
        fullVertexData.push_back(Attributes.normals[(vData.normal_index * 3) + 1]);
        fullVertexData.push_back(Attributes.normals[(vData.normal_index * 3) + 2]);

        fullVertexData.push_back(Attributes.texcoords[(vData.texcoord_index * 2)]);
        fullVertexData.push_back(Attributes.texcoords[(vData.texcoord_index * 2) + 1]);

        fullVertexData.push_back(tangents[i].x);
        fullVertexData.push_back(tangents[i].y);
        fullVertexData.push_back(tangents[i].z);

        fullVertexData.push_back(bitangents[i].x);
        fullVertexData.push_back(bitangents[i].y);
        fullVertexData.push_back(bitangents[i].z);

    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

    //Change 5 to 8. position XYZ, normal XYZ, UV = 8


    GLintptr normPtr = 3 * sizeof(GLfloat);
    GLintptr uvPtr = 6 * sizeof(GLfloat);
    GLintptr tangentPtr = 8 * sizeof(GLfloat);
    GLintptr bitangentPtr = 11 * sizeof(GLfloat);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 14 * sizeof(GLfloat), (void*)normPtr);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)uvPtr);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)tangentPtr);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)bitangentPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    Transform = glm::translate(identity_matrix4, glm::vec3(0, 0, 100)); // x,y,z
    //scale
    Transform = glm::scale(Transform, glm::vec3(0.04f, 0.04f, 0.04f)); // x,y,z
    //rotate
    Transform = glm::rotate(Transform, glm::radians(90.f), glm::vec3(1, 0, 0));
    Transform = glm::rotate(Transform, glm::radians(180.f), glm::vec3(0, 1, 0));//theta, x,y,z

    pivot = Transform;
}

void Model::SetupTexture(string path, string path2)
{
    stbi_set_flip_vertically_on_load(true);
    tex_bytes = stbi_load(path.c_str(), &img_width, &img_height, &colorChannels, 0);

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    norm_tex_bytes = stbi_load(path2.c_str(), &norm_img_width, &norm_img_height, &norm_colorChannels, 0);

    glGenTextures(1, &normalMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, norm_img_width, norm_img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, norm_tex_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(norm_tex_bytes);
}

void Model::Draw(GLuint ShaderProgram, bool perspective, PerspectiveCamera perspectiveCamera, OrthoCamera orthoCamera, Light light)
{
    light.ApplyLight(ShaderProgram);

    if (perspective)
    {
        perspectiveCamera.ApplyCameraPosition(ShaderProgram);
        perspectiveCamera.ApplyProjection(ShaderProgram);
        perspectiveCamera.ApplyView(ShaderProgram);
    }
    else if (!perspective)
    {
        
        orthoCamera.ApplyCameraPosition(ShaderProgram);
        orthoCamera.ApplyProjection(ShaderProgram);
        orthoCamera.ApplyView(ShaderProgram);
    }

    unsigned int TransformLoc = glGetUniformLocation(ShaderProgram, "transform");
    glUniformMatrix4fv(TransformLoc, 1, GL_FALSE, glm::value_ptr(Transform));

    glActiveTexture(GL_TEXTURE0);
    GLuint tex0Address = glGetUniformLocation(ShaderProgram, "tex0");
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(tex0Address, 0);

    glActiveTexture(GL_TEXTURE1);
    GLuint tex1Address = glGetUniformLocation(ShaderProgram, "norm_tex");
    glBindTexture(GL_TEXTURE_2D, this->normalMap);
    glUniform1i(tex1Address, 1);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 14);
}

GLuint Model::GetTexture()
{
    return texture;
}

void Model::DeleteBuffers()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Model::PlayerControls(unordered_map<string, bool> input, float deltaTime, PerspectiveCamera camera) 
{

    if (input["W"] == true)
    {
        Transform = glm::translate(Transform, glm::vec3(0, -1 * SPEED * deltaTime,0));
    }

    if (input["S"] == true)
    {
        Transform = glm::translate(Transform, glm::vec3(0, 1 * SPEED * deltaTime, 0));
    }

    if (input["A"] == true)
    {
        Transform = glm::rotate(Transform, glm::radians(TURN_SPEED * deltaTime), glm::vec3(0, 0, 1));
    }

    if (input["D"] == true)
    {
        Transform = glm::rotate(Transform, glm::radians(TURN_SPEED * deltaTime), glm::vec3(0, 0, -1));
    }

    if (input["Q"] == true)
    {
        Transform = glm::translate(Transform, glm::vec3(0, 0, 1 * VERTICAL_SPEED * deltaTime));
    }

    if (input["E"] == true)
    {
        Transform = glm::translate(Transform, glm::vec3(0, 0, -1 * VERTICAL_SPEED * deltaTime));
    }

}

glm::mat4 Model::GetTransform()
{
    return this->Transform;
}

glm::mat4 Model::GetPivot()
{
    return this->pivot;
}




