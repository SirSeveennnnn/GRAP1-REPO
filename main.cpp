#pragma once

#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Model.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include"tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#include<string>
#include<iostream>

#include "OrthoCamera.h"
#include "PerspectiveCamera.h"
#include "Light.h"
#include "Model.h"
#include "MyShaders.h"
#include "MySkybox.h"
#include "Planet.h"
#include"Debris.h"

#include<unordered_map>

using namespace std;

//Inputs
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


float delta;
bool firstMouse = true;
double lastX, lastY;
glm::vec2 mousePos;
bool usePerspective = true;
bool rotateObject = true;

unordered_map<string, bool> input;

int main(void)
{

    input["W"] = false;
    input["A"] = false;
    input["S"] = false;
    input["D"] = false;
    input["Q"] = false;
    input["E"] = false;
    input["Space"] = true;
    input["mousePressed"] = false;
    input["pointLight"] = true;
    mousePos.x = 0;
    mousePos.y = 0;
    
    float width = 800;
    float height = 800;
    GLFWwindow* window;
    

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Jae Macuha", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, width, height);
    
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    glEnable(GL_DEPTH_TEST);

    Light light;
    OrthoCamera orthoCamera;
    PerspectiveCamera perspectiveCamera;

    Model* spaceship = new Model("3D/spaceship.obj");
    spaceship->SetupTexture("Textures/spaceship.jpg", "Textures/spaceship_normal.jpg");
    MyShaders* shader_Spaceship = new MyShaders("Shaders/shader.vert", "Shaders/shader.frag");

    MySkybox* skybox = new MySkybox();
    MyShaders* shader_Skybox = new MyShaders("Shaders/skybox.vert", "Shaders/skybox.frag");

    Planet* planet = new Planet("3D/Venus.obj");
    planet->SetupTexture("Textures/Venus1.png");
    MyShaders* planetShader = new MyShaders("Shaders/planet.vert", "Shaders/planet.frag");

    Debris* asteroid = new Debris("3D/asteroid.obj");
    asteroid->SetupTexture("Textures/asteroid.jpg");
    asteroid->SetScale(glm::vec3(0.01f,0.01f,0.01f));
    asteroid->SetRadius(80);
    MyShaders* debrisShader = new MyShaders("Shaders/debris.vert", "Shaders/debris.frag");

    Debris* rock = new Debris("3D/rock.obj");
    rock->SetupTexture("Textures/rock.jpg");
    rock->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
    rock->SetRadius(100);

    Debris* lavarock = new Debris("3D/lavarock.obj");
    lavarock->SetupTexture("Textures/lavarock.jpg");
    lavarock->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
    lavarock->SetRadius(90);

    Debris* smallrock = new Debris("3D/smallrock.obj");
    smallrock->SetupTexture("Textures/smallrock.jpg");
    smallrock->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
    smallrock->SetRadius(50);

    Debris* bigrock = new Debris("3D/bigrock.obj");
    bigrock->SetupTexture("Textures/bigrock.jpg");
    bigrock->SetScale(glm::vec3(10.f, 10.f, 10.f));
    bigrock->SetRadius(200);
    
    asteroid->Orbit(delta, 20, 0.5f, 0.1f, 0);
    rock->Orbit(delta, 300, 0.65f, 0.12f, 25);
    lavarock->Orbit(delta, 200, 0.4f, 0.08f, -30);
    smallrock->Orbit(delta, 40, 0.2f, 0.01f, 50);
    bigrock->Orbit(delta, 120, 0.7f, 0.1f, -10);

    ////////////////////////////////////////////////////////////////////////
    //Transform
    // Delta Time
    float lastTime = glfwGetTime();

    light.SetLightPosition(glm::vec3(0, 0, 0));
    light.SetLightBrightness(25000);
    light.SetLightColor(0.50f, 0.95f, 0.50f);

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Delta Time
        //current time
        GLfloat currTime = glfwGetTime();
        delta = currTime - lastTime;

        if (usePerspective)
        {
            perspectiveCamera.FollowTarget(spaceship->GetTransform(), input, mousePos);
            perspectiveCamera.UpdateViewMatrix(spaceship->GetTransform());
            spaceship->PlayerControls(input, delta, perspectiveCamera);
        }
        else if(!usePerspective)
        {
            orthoCamera.TopView(input, delta);
        }

        if (input["pointLight"])
        {
            light.SetLightPosition(glm::vec3(0, 0, 0));
            light.SetLightBrightness(25000);
            light.SetLightColor(0.50f, 0.95f, 0.50f);
            light.SetPointLight(input["pointLight"]);
        }
        else if(!input["pointLight"])
        {
            glm::vec3 directional;
            directional.x = spaceship->GetTransform()[3][0];
            directional.y = spaceship->GetTransform()[3][1];
            directional.z = spaceship->GetTransform()[3][2];

            directional = glm::normalize(directional);

            light.SetLightColor(1.f, 1.f, 1.f);
            light.SetPointLight(input["pointLight"]);
            light.SetLightPosition(directional);
            light.SetDirectional(directional);
        }
       
        //SKYBOX
        shader_Skybox->UseShader();
        skybox->Draw(shader_Skybox->GetShaderProgram(), usePerspective, perspectiveCamera, orthoCamera);
        
        //SPACESHIIP
        shader_Spaceship->UseShader();
        spaceship->Draw(shader_Spaceship->GetShaderProgram(), usePerspective, perspectiveCamera, orthoCamera, light);
        //Planet
        planetShader->UseShader();
        planet->Draw(planetShader->GetShaderProgram(), usePerspective, perspectiveCamera, orthoCamera);

        //Debris
        debrisShader->UseShader();
        asteroid->Draw(debrisShader->GetShaderProgram(), usePerspective, perspectiveCamera, orthoCamera, light);
        

        debrisShader->UseShader();
        rock->Draw(debrisShader->GetShaderProgram(), usePerspective, perspectiveCamera, orthoCamera, light);
        

        debrisShader->UseShader();
        lavarock->Draw(debrisShader->GetShaderProgram(), usePerspective, perspectiveCamera, orthoCamera, light);
        

        debrisShader->UseShader();
        smallrock->Draw(debrisShader->GetShaderProgram(), usePerspective, perspectiveCamera, orthoCamera, light);
        

        debrisShader->UseShader();
        bigrock->Draw(debrisShader->GetShaderProgram(), usePerspective, perspectiveCamera, orthoCamera, light);
        

        if (input["Space"] == true)
        {
            asteroid->Orbit(delta, 20, 0.5f, 0.1f, 0);
            rock->Orbit(delta, 300, 0.65f, 0.12f, 25);
            lavarock->Orbit(delta, 200, 0.4f, 0.08f, -30);
            smallrock->Orbit(delta, 40, 0.2f, 0.01f, 50);
            bigrock->Orbit(delta, 120, 0.7f, 0.1f, -10);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        

        lastTime = currTime; // delta time
    }
    //Free Memory
    spaceship->DeleteBuffers();
    skybox->DeleteBuffers();
    planet->DeleteBuffers();
    asteroid->DeleteBuffers();

    glfwTerminate();
    return 0;

}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        input["W"] = true;
    }
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        input["W"] = false;
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        input["A"] = true;
    }
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        input["A"] = false;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        input["S"] = true;
    }
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        input["S"] = false;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        input["D"] = true;
    }
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    {
        input["D"] = false;
    }

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        input["Q"] = true;
    }
    else if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
    {
        input["Q"] = false;
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        input["E"] = true;
    }
    else if (key == GLFW_KEY_E && action == GLFW_RELEASE)
    {
        input["E"] = false;
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        input["pointLight"] = !input["pointLight"];
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        input["Space"] = !input["Space"];
        
    }
    
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        usePerspective = !usePerspective;
    }

    
    

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){


    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        input["mousePressed"] = true;
         
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        input["mousePressed"] = false;
        firstMouse = true;
    }
    
    
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (input["mousePressed"] == true)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.3f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        mousePos.x += xoffset;
        mousePos.y += yoffset;

       
    }
   
}

