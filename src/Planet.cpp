#include "Planet.h"

Planet::Planet(string name,const char* texturePath, float size, float orbitRadius, float orbitSpeed, float rotationSpeed, Planet* parent,GLuint shaderID) {
    this->name = name;
    this->texturePath = texturePath;
    this->size = size;
    this->orbitRadius = orbitRadius;
    this->orbitSpeed = orbitSpeed;
    this->rotationSpeed =rotationSpeed;
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
    cout << "Destroyed planet" << endl; 
}

void Planet::Draw() {
    glUseProgram(shaderID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderID,"texture"), 0);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f)); 
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
    model = glm::scale(model, glm::vec3(0.2f));
    glm::mat4 view; 
    view = glm::lookAt(cameraPos, cameraPos + cameraFront,cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f); 
   
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Planet::LoadTexture() {
    glGenTextures(1,&texture); 
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    
    stbi_set_flip_vertically_on_load(true); 
    int width,height, nrChannels, width2, height2; 
    unsigned char * data = stbi_load(texturePath, &width, &height, &nrChannels, 0); 
    if (data) {
        glBindTexture(GL_TEXTURE_2D, texture); 
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB; 
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