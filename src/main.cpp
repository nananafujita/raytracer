#include <GLFW/glfw3.h>

#include "Model.h"

#define FOV 60.0
#define WIDTH 1280
#define HEIGHT 960

void error_callback(int error, const char* description) {
    printf("GLFW Error %d: %s\n", error, description);
    fflush(stdout);
}

void display()
{
    double origin[3] = {0, 0, 0};
    float angle = (FOV / 2) * M_PI / 180.0;
}

int main(int argc, char* argv[])
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    Model model(argv[1]);
    if (!model.loadOBJ()) {
        glfwTerminate();
        return -1;
    }
    printf("model size: %zu", model.getTriangles().size());

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}