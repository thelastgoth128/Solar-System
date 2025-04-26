#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "resources/shaders/shader.cpp"
#include "Planet.cpp"

using namespace std;


int main () {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Solar-System", NULL, NULL);
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

    glViewport(0, 0, 800, 600);


    Shader ourShader("C:\\Users\\thind\\Documents\\projects\\Solar-System\\src\\resources\\shaders\\vertex.vs","C:\\Users\\thind\\Documents\\projects\\Solar-System\\src\\resources\\shaders\\fragment.fss");
    Planet Sun("Sun","C:\\Users\\thind\\Documents\\projects\\Solar-System\\src\\resources\\PlanetTextureMaps\\sunmap.jpg",1.0f,0.0f,0.2f,0.2f,nullptr,ourShader.ID);
    Planet Earth("Earth","C:\\Users\\thind\\Documents\\projects\\Solar-System\\src\\resources\\PlanetTextureMaps\\earthmap1k.jpg",0.4f,1.0f,0.3f,0.2f,nullptr,ourShader.ID);

    while(!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Sun.Draw();
        Earth.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}