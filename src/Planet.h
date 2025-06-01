#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "resources/Sphere/Sphere.h"
#include "resources/shaders/shader.h"

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
    void Orbit(float deltaTime);
    GLuint orbitVAO, orbitVBO;
    int orbitSegments = 100;
    void drawOrbit(GLuint orbitVAO, int segments, glm::mat4 view, glm::mat4 projection, GLuint shaderID);
    void setupOrbitVAO(std::vector<glm::vec3>& orbitPoints, GLuint& VAO, GLuint& VBO);
    std::vector<glm::vec3> generateOrbitPoints(glm::vec3 parentPosition, float orbitRadius, int segments);
    void drawOrbit(glm::mat4 view, glm::mat4 projection, GLuint shaderID) ;
    void drawStars(float size,glm::mat4 view, glm::mat4 projection);
    void updateRotation(float rotationSpeed);
    void updateOrbitSpeed (float orbitSpeed);
};