#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "Model.h"

#define FOV 60.0
const int WIDTH = 512; //1280
const int HEIGHT = 512; //960

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
    /*for (int j=HEIGHT-1; j>=0; j--){
        for (int i=0; i<WIDTH; i++) {
            auto r = static_cast<unsigned char>(255.999 * (double(i) / (WIDTH - 1)));
            auto g = static_cast<unsigned char>(255.999 * (double(j) / (HEIGHT - 1)));
            auto b = 0;
            pixels.push_back(r);
            pixels.push_back(g);
            pixels.push_back(b);
        }
    }*/
    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.0f); glVertex2i(0, 0);           // Bottom-left (black)
        glColor3f(1.0f, 0.0f, 0.0f); glVertex2i(WIDTH, 0);       // Bottom-right (red)
        glColor3f(1.0f, 1.0f, 0.0f); glVertex2i(WIDTH, HEIGHT);  // Top-right (yellow)
        glColor3f(0.0f, 1.0f, 0.0f); glVertex2i(0, HEIGHT);      // Top-left (green)
    glEnd();
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

    // Fill pixels iteratively like your original code
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            double r = double(i) / (WIDTH - 1);
            double g = double(j) / (HEIGHT - 1);
            double b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            int index = (j * WIDTH + i) * 3;
            pixels[index + 0] = ir;
            pixels[index + 1] = ig;
            pixels[index + 2] = ib;
        }
    }

    /*Model model(argv[1]);
    if (!model.loadOBJ()) {
        glfwTerminate();
        return -1;
    }
    printf("model size: %zu", model.getTriangles().size());*/

    //generateGradient(pixels);
    glViewport(0, 0, WIDTH, HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        GLint windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);
        handleInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}