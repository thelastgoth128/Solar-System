///////////////////////////////////////////////////////////////////////////////
// Planet.cpp
// ==========
//Most implementantion was inspired from the youtube video "https://youtu.be/22pcxHjbqOM?si=_ICO4rmXhBcVKlDy"
// source for sphere From: https://songho.ca/opengl/gl_sphere.html  
// Planet constructor that initializes the planet attributes, initializes the buffers and loads textures
// Planet deconstructor to destroy the planets
// Draw function = gets view and projection matrices as inputs and computes th model matrix then loads then into shader
// the Draw function also draw the sphere with appropriate texture then computes orbitsPoints
// LoadTexture function loads the texture, computes its channels and passes it to the shader 
// Orbit function handles both orbit and rotation for every initiated planet
// UpadteRotation gets the speed as input the update the rotation speed 
// UpdateOrbit speed updates the orbit speed of a planet
// generateOrbitPoints given a parent position it computes the orbit points code example form youtube "https://youtu.be/Wq_VmYKqMzg?si=-C2e2mFyDB1lCKHH"
// drawOrbit checks if the planet has a parent then calls the drawOrbit function
// drawOrbit draws the paths
// drawStars draws the sphere with a star texture
//
//  AUTHOR: CYRUS THINDWA (thindwacyrus86@gmail.com)
// CREATED: 2025-05-07
///////////////////////////////////////////////////////////////////////////////

#include "Planet.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "../dependencies/include/stb_image.h"

Planet::Planet(string name,const char* texturePath, float size, float orbitRadius, float orbitSpeed, float rotationSpeed, Planet* parent,GLuint shaderID) {
    this->name = name;
    this->texturePath = texturePath;
    this->size = size;
    this->orbitRadius = orbitRadius;
    this->orbitSpeed = orbitSpeed;
    this->rotationSpeed = rotationSpeed;
    this->parent = parent;
    this->shaderID = shaderID;
    

    position = glm::vec3(0.0f);
    rotationAngle = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex data (interleaved V/N/T)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sphere.getInterleavedVertexSize(), sphere.getInterleavedVertices(), GL_STATIC_DRAW);

    // Index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexSize(), sphere.getIndices(), GL_STATIC_DRAW);

    // Vertex Attributes
    GLsizei stride = sphere.getInterleavedStride();

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    LoadTexture();
}

Planet::~Planet() {
    glDeleteTextures(1, &texture);

    cout << "deleting a planet" << endl;
}

void Planet::Draw(glm::mat4 view, glm::mat4 projection) {
    glUseProgram(shaderID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderID,"texSample"), 0);

    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::translate(model, glm::vec3(position)); 
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); 
    model = glm::scale(model, glm::vec3(size));
   
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (parent) {
        std::vector<glm::vec3> orbitPoints = generateOrbitPoints(parent->position, orbitRadius, orbitSegments);
        setupOrbitVAO(orbitPoints, orbitVAO, orbitVBO);
    }
}

void Planet::LoadTexture() {
    glGenTextures(1,&texture); 
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    
    stbi_set_flip_vertically_on_load(true); 
    int width,height, nrChannels; 
    unsigned char * data = stbi_load(texturePath, &width, &height, &nrChannels, 0); 

    if (data) {
        glBindTexture(GL_TEXTURE_2D, texture);
        GLenum format; 
        if (nrChannels == 4) {
            format = GL_RGBA;
        }else if (nrChannels == 3) {
            format = GL_RGB;
        }else if (nrChannels == 2) {
            format = GL_RG;
        }else if (nrChannels == 1) {
            format = GL_RED;
        } 
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); 
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shaderID, "texture"), 0); 

    } else { 
        cout << "Failed to load texture1" << endl; 

    } 
    stbi_image_free(data); 
}

void Planet::Orbit(float deltaTime) {
    orbitAngle += orbitSpeed * deltaTime;
    if (orbitAngle >= 360.0f) {
        orbitAngle -= 360.0f;
    }

    if (parent) {
        glm::vec3 parentPos = parent->position;
        position.x = parentPos.x + orbitRadius * cos(glm::radians(orbitAngle));
        position.y = parentPos.y;
        position.z = parentPos.z + orbitRadius * sin(glm::radians(orbitAngle));
    }

    rotationAngle += rotationSpeed * deltaTime;
    if (rotationAngle >= 360.0f) rotationAngle -= 360.0f;
}

void Planet::updateRotation(float Speed) {
    rotationSpeed = Speed;
}

void Planet::updateOrbitSpeed(float speed) {
    orbitSpeed = speed;
}

std::vector<glm::vec3>Planet::generateOrbitPoints(glm::vec3 parentPos, float orbitRadius, int segments) {
    std::vector<glm::vec3> points;
    for (int i = 0; i < segments; ++i) {
        float angle = glm::radians((360.0f / segments) * i);
        float x = parentPos.x + orbitRadius * cos(angle);
        float z = parentPos.z + orbitRadius * sin(angle);
        points.emplace_back(glm::vec3(x, parentPos.y, z)); 
    }
    return points;
}

void Planet::setupOrbitVAO(std::vector<glm::vec3>& orbitPoints, GLuint& VAO, GLuint& VBO) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, orbitPoints.size() * sizeof(glm::vec3), orbitPoints.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
}

void Planet::drawOrbit(glm::mat4 view, glm::mat4 projection, GLuint shaderID) {
    if (parent) {
        drawOrbit(orbitVAO, orbitSegments, view, projection, shaderID);
    }
}

void Planet::drawOrbit(GLuint orbitVAO, int segments, glm::mat4 view, glm::mat4 projection, GLuint shaderID) {
    glUseProgram(shaderID);

    glm::mat4 model = glm::mat4(1.0f); // Identity matrix for orbit path
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(orbitVAO);
    glDrawArrays(GL_LINE_LOOP, 0, segments);
    glBindVertexArray(0);
}

void Planet::drawStars(float size,glm::mat4 view, glm::mat4 projection){
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex data (interleaved V/N/T)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sphere.getInterleavedVertexSize(), sphere.getInterleavedVertices(), GL_STATIC_DRAW);

    // Index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexSize(), sphere.getIndices(), GL_STATIC_DRAW);

    // Vertex Attributes
    GLsizei stride = sphere.getInterleavedStride();

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    LoadTexture();

    glUseProgram(shaderID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderID,"texture"), 0);

    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::translate(model, glm::vec3(position)); 
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); 
    model = glm::scale(model, glm::vec3(size));
   
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

