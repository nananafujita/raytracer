#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "Model.h"

#define FOV 60.0
#define WIDTH 256 //1280
#define HEIGHT 256 //960

void error_callback(int error, const char* description) {
    printf("GLFW Error %d: %s\n", error, description);
    fflush(stdout);
}

void display()
{
    double origin[3] = {0, 0, 0};
    float angle = (FOV / 2) * M_PI / 180.0;
}

void generateGradient(std::vector<unsigned char>& pixels) {
    for (int j=0; j<HEIGHT; j++){
        for (int i=0; i<WIDTH; i++) {
            auto r = static_cast<unsigned char> (255.999 * (double(i) / (WIDTH - 1)));
            auto g = static_cast<unsigned char> (255.999 * (double(j) / (HEIGHT - 1)));
            auto b = 0;
            pixels.push_back(r);
            pixels.push_back(g);
            pixels.push_back(b);
        }
    }
}

int main(int argc, char* argv[])
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        std::cerr << "Error creating GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    /*Model model(argv[1]);
    if (!model.loadOBJ()) {
        glfwTerminate();
        return -1;
    }
    printf("model size: %zu", model.getTriangles().size());*/

    std::vector<unsigned char> pixels;
    pixels.reserve(WIDTH * HEIGHT * 3);
    generateGradient(pixels);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}