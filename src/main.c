#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "render.h"


void init()
{
  glMatrixMode(GL_PROJECTION);
  glOrtho(0,WIDTH,0,HEIGHT,1,-1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
}


void plot_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
  glColor3f(((float)r) / 255.0f, ((float)g) / 255.0f, ((float)b) / 255.0f);
  glVertex2i(x,y);
}

void draw(Vec3* intensities)
{
  for(unsigned int x=0; x<WIDTH; x++)
  {
    glPointSize(2.0);  
    // Do not worry about this usage of OpenGL. This is here just so that we can draw the pixels to the screen,
    // after their R,G,B colors were determined by the ray tracer.
    glBegin(GL_POINTS);
    for(unsigned int y=0; y<HEIGHT; y++)
    {
      // A simple R,G,B output for testing purposes.
      // Modify these R,G,B colors to the values computed by your ray tracer.
      unsigned char r = fabs(intensities[y + x * HEIGHT].x) * 255.0;
      unsigned char g = fabs(intensities[y + x * HEIGHT].y) * 255.0;
      unsigned char b = fabs(intensities[y + x * HEIGHT].z) * 255.0;

      plot_pixel(x, y, r, g, b);
    }
    glEnd();
    glFlush();
  }
  printf("Ray tracing completed.\n"); 
  fflush(stdout);
}

void key_callback(GLFWwindow* wind, int key, int scancode, int action, int mods) 
{
    switch(key)
    {
        case 256:
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

    if (load_scene(argv[1]) != 0) {
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
    Vec3* intensities = malloc(sizeof(Vec3) * WIDTH * HEIGHT);
    render(intensities); 
    draw(intensities);

    while (!glfwWindowShouldClose(window)) {

        //idle();    

        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }
    free(intensities);
    glfwDestroyWindow(window);
    glfwTerminate();
}