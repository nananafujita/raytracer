#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "render.h"

void init()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,WIDTH,0,HEIGHT,1,-1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(1,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void plot_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
  glColor3f(((float)r) / 255.0f, ((float)g) / 255.0f, ((float)b) / 255.0f);
  glVertex2i(x,y);
}

void draw(Vec3* intensities)
{
  for(unsigned int i=0; i<HEIGHT; i++)
  {
    glPointSize(2.0);  
    // Do not worry about this usage of OpenGL. This is here just so that we can draw the pixels to the screen,
    // after their R,G,B colors were determined by the ray tracer.
    glBegin(GL_POINTS);
    for(unsigned int j=0; j<WIDTH; j++)
    {
      // A simple R,G,B output for testing purposes.
      // Modify these R,G,B colors to the values computed by your ray tracer.
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

GLuint draw_texture(Vec3* intensities)
{
    printf("generating texture...");
    GLubyte* image_buffer = malloc(WIDTH * HEIGHT * 3);
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            image_buffer[(i * WIDTH + j) * 3 + 0] = (GLubyte)intensities[i * WIDTH + j].x * 255.0;
            image_buffer[(i * WIDTH + j) * 3 + 1] = (GLubyte)intensities[i * WIDTH + j].y * 255.0;
            image_buffer[(i * WIDTH + j) * 3 + 2] = (GLubyte)intensities[i * WIDTH + j].z * 255.0;
        }
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, image_buffer);

    free(image_buffer);
    printf("done.\n");
    return tex;
}

void display_texture(GLuint tex)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(WIDTH, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(WIDTH, HEIGHT);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, HEIGHT);
    glEnd();

    glDisable(GL_TEXTURE_2D);
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
    GLuint texture = draw_texture(intensities);
    draw(intensities);

    while (!glfwWindowShouldClose(window)) {
        //display_texture(texture);
        draw(intensities);
        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }
    free(intensities);
    glfwDestroyWindow(window);
    glfwTerminate();
}