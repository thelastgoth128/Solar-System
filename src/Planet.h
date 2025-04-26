#pragma once
#include <iostream>
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "resources/Sphere/Sphere.cpp"
#include "resources/shaders/shader.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

using namespace std;

class Planet {
    private:
    string name;
    float orbitRadius;
    float orbitAngle;
    float orbitSpeed;
    float rotationAngle;
    float rotationSpeed;
    float size;
    glm::vec3 position;
    GLuint VAO, VBO, EBO;
    const char* texturePath;
    Planet* parent;
    GLuint texture;
    GLuint shaderID;
    Sphere sphere;

    public:
    Planet(string name,const char* texturePath, float size, float orbitRadius, float orbitSpeed, float rotationSpeed, Planet* parent,GLuint shaderID);
    ~Planet();
    void Draw(glm::mat4 view, glm::mat4 projection);
    void LoadTexture();
    void Orbit(float orbitAngle, float rotationAngle);
};