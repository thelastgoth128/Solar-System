#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "resources/shaders/shader.cpp"
#include "Planet.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f; //Time between current frame and last frame
float lastFrame = 0.0f; //Time of last frame
bool animate = true;
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window,true);
    }

    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (animate == true) {
            animate = false;
            lastFrame = 0.0f;
        }
        else {
            animate = true;
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }
    } 
    
    if (animate) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
}


float lastX = 400, lastY = 300;
float yaw, pitch;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos,double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch <-89.0f)
        pitch =-89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
};

//zooming 
float Zoom =45.0f;
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
    
}


int main () {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Solar-System", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 1920, 1080);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("../src\\resources\\shaders\\vertex.vs","../src\\resources\\shaders\\fragment.fss");
    Shader moonShader("../src\\resources\\shaders\\vertex.vs","../src\\resources\\shaders\\moonFragment.fss");
    Shader pathShader("../src\\resources\\shaders\\vertex.vs","../src\\resources\\shaders\\pathFragment.fss");

    Planet Sun("Sun","../src\\resources\\PlanetTextureMaps\\sunmap.jpg",2.0f,0.0f,0.0f,3.0f,nullptr,ourShader.ID);
    Planet Mercury("Mercury","../src\\resources\\PlanetTextureMaps\\mercurymap.jpg",0.4f,4.0f,10.0f,5.0f,&Sun,ourShader.ID);
    Planet Venus("Venus","../src\\resources\\PlanetTextureMaps\\venusmap.jpg",0.8f,8.0f,8.0f,10.0f,&Sun,ourShader.ID);
    Planet Earth("Earth","../src\\resources\\PlanetTextureMaps\\earthmap1k.jpg",1.4f,12.0f,6.0f,15.0f,&Sun,ourShader.ID);
    Planet Moon("Moon","../src\\resources\\PlanetTextureMaps\\moonmap1k.jpg",0.2f,2.0f,8.0f,10.0f,&Earth,moonShader.ID);
    Planet Mars("Mars","../src\\resources\\PlanetTextureMaps\\marsmap1k.jpg",1.7f,18.0f,4.0f,20.0f,&Sun,ourShader.ID);
    Planet Stars("Stars","../src\\resources\\PlanetTextureMaps\\Stars.jpg",2.0f,12.0f,5.0f,50.0f,&Sun,ourShader.ID);
    
    while(!glfwWindowShouldClose(window)) {

        processInput(window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view; 
        view = glm::lookAt(cameraPos, cameraPos + cameraFront,cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

        Stars.drawStars(40,view, projection);
        

        Sun.Orbit(deltaTime);
        Mercury.Orbit(deltaTime);
        Venus.Orbit(deltaTime);
        Earth.Orbit(deltaTime);
        Moon.Orbit(deltaTime);
        Mars.Orbit(deltaTime);

        Sun.drawOrbit(view, projection, pathShader.ID);
        Mercury.drawOrbit(view, projection, pathShader.ID);
        Venus.drawOrbit(view, projection, pathShader.ID);
        Earth.drawOrbit(view, projection, pathShader.ID);
        Mars.drawOrbit(view, projection, pathShader.ID);
        Moon.drawOrbit(view, projection, moonShader.ID);

        Sun.Draw(view, projection);
        Mercury.Draw(view, projection);
        Venus.Draw(view, projection);
        Earth.Draw(view, projection);
        Moon.Draw(view, projection);
        Mars.Draw(view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}