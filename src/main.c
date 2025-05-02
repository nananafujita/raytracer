#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "scene.h"

int main(int argc, char* argv[]) 
{

    if (!glfwInit()) {
        printf("Failed to initialize GLFW. Exiting program.\n");
        return -1;
    }

    if (load_scene(argv[1]) != 0) {
        return -1;
    }
}