#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "Model.h"

#define FOV 60.0
const int WIDTH = 512; //1280
const int HEIGHT = 512; //960
GLuint textureID;

void error_callback(int error, const char* description) {
    printf("GLFW Error %d: %s\n", error, description);
    fflush(stdout);
}

void initOpenGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    double origin[3] = {0, 0, 0};
    float angle = (FOV / 2) * M_PI / 180.0;
}

void generateGradient(std::vector<unsigned char>& pixels) {
    for (int j=HEIGHT-1; j>=0; j--){
        for (int i=0; i<WIDTH; i++) {
            auto r = static_cast<unsigned char>(255.999 * (double(i) / (WIDTH - 1)));
            auto g = static_cast<unsigned char>(255.999 * (double(j) / (HEIGHT - 1)));
            auto b = 0;
            int index = (j * WIDTH + i) * 3;
            pixels[index]     = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
        }
    }
}

void handleInput(GLFWwindow* window) 
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)   glfwSetWindowShouldClose(window, true);
}

int main(int argc, char* argv[])
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        std::cerr << "Error creating GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    //initOpenGL();

    std::vector<unsigned char> pixels(WIDTH * HEIGHT * 3);
    generateGradient(pixels);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Enable 2D texturing
    glEnable(GL_TEXTURE_2D);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &texture); /*

    while (!glfwWindowShouldClose(window))
    {
        GLint windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


        handleInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glRasterPos2i(0, 0);
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }*/

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}