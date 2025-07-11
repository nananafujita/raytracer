#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "camera.h"

int drawn = 0; // flag to make sure rendering only happens once

// initializes OpenGL settings
void init()
{
  glMatrixMode(GL_PROJECTION);
  glOrtho(0,WIDTH,0,HEIGHT,1,-1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(1,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
}

// plots single pixel as a point using rgb
void plot_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
  // normalize color to [0.0, 1.0]
  glColor3f(((float)r) / 255.0f, ((float)g) / 255.0f, ((float)b) / 255.0f);
  glVertex2i(x,y);
}

// loop through viewport and draw image from 2d array of colors
void draw(Vec3* intensities)
{
    if (!drawn) {
        for(unsigned int i=0; i<HEIGHT; i++)
        {
            glPointSize(2.0);  
            glBegin(GL_POINTS);
            for(unsigned int j=0; j<WIDTH; j++)
            {
              unsigned char r = fabs(intensities[i * WIDTH + j].x) * 255.0;
              unsigned char g = fabs(intensities[i * WIDTH + j].y) * 255.0;
              unsigned char b = fabs(intensities[i * WIDTH + j].z) * 255.0;
  
              plot_pixel(j, i, r, g, b);
            }
            glEnd();    
            glFlush();  
        }
        printf("Ray tracing completed.\n"); 
        fflush(stdout);
    }
    drawn = 1;  // prevent redrawing image
}

void key_callback(GLFWwindow* wind, int key, int scancode, int action, int mods) 
{
    switch(key)
    {
        case 256:     // ESC
            exit(0);
            break;
    }
} 

int main(int argc, char* argv[]) 
{
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW. Exiting program.\n");
        return -1;
    }

    if (Scene::instance().load_scene(argv[1]) != 0) {
        fprintf(stderr, "Error when parsing file. See above comments for details.\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Tracer", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window. Exiting program.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    init();

    // stores pixel colors (intensities) in a 1d array
    Vec3* intensities = (Vec3*)malloc(sizeof(Vec3) * WIDTH * HEIGHT);
    render(intensities); 
    draw(intensities);
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); 
    }
    free(intensities);
    glfwDestroyWindow(window);
    glfwTerminate();
}
