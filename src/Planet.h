#pragma once
#include <iostream>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "resources/Sphere/Sphere.cpp"
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
    void Draw();
    void LoadTexture();
    void Orbit(float orbitAngle, float rotationAngle);
};