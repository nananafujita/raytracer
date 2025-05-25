#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "render.h"

#define WIDTH 400
#define HEIGHT 300
#define FOV 60.0

void key_callback(GLFWwindow* wind, int key, int scancode, int action, int mods) 
{
    switch(key)
    {
        case 256:
            exit(0);
            break;
    }
} 

void display()
{
    double origin[3] = {0, 0, 0};
    double half_angle = (FOV / 2) * M_PI / 180.0;                   // angle (in radians) between center and top of image plane
    double x = (float) WIDTH / (float) HEIGHT * tan(half_angle);    // half width of image plane
    double y = tan(half_angle);                                     // half height of image plane
    double z = -1.0;
    double step_width = 2 * x / WIDTH;                              // step width on the image plane
    double step_height = 2 * y / HEIGHT;                                

    struct Pixel closest_object;
    // loop through camera rays
    for (int i=0; i<WIDTH; i++) {
        for (int j=0; j<HEIGHT; j++) {
            int intersected = -1;
            double closest_dist = 0.0;

            double direction[3] = {i * step_width - x, j * step_height - y, z};
            normalize(direction);

            int closest_is_sphere;
            int closest_index;
            double closest_point[3];
            double closest_normal[3];
            double closest_barycentric[3];

            double hit_point[3];
            double hit_normal[3];
            double hit_barycentric[3];

            double intensity[3] = {ambient_light[0], ambient_light[1], ambient_light[2]};
            for (int s=0; s<num_spheres; s++) {
                if (hit_sphere(spheres[s], origin, direction, hit_point, hit_normal)) {
                    if (intersected != 0 || squared_length(hit_point) < closest_dist) { // camera is at origin, so we can do length() directly
                        closest_is_sphere = 1;
                        closest_index = s;
                        for (int k=0; k<3; k++) {   
                            closest_point[k] = hit_point[k];
                            closest_normal[k] = hit_normal[k];
                            closest_barycentric[k] = -1;
                        }
                        intersected = 0;
                        closest_dist = squared_length(hit_point);
                    }
                }
            }
            for (int t=0; t<num_triangles; t++) {
                if (hit_triangle(triangles[t], origin, direction, hit_point, hit_normal, hit_barycentric)) {
                    if (intersected != 0 || squared_length(hit_point) < closest_dist) {
                        closest_is_sphere = 0;
                        closest_index = t;
                        for (int k=0; k<3; k++) {   
                            closest_point[k] = hit_point[k];
                            closest_normal[k] = hit_normal[k];
                            closest_barycentric[k] = hit_barycentric[k];
                        }
                        intersected = 0;
                        closest_dist = squared_length(hit_point);
                    }
                }
            }
            printf("%i %i\n", j, i);
            if (intersected == 0) {
                struct Pixel p;
                closest_pixel(closest_is_sphere, closest_index, closest_point, closest_normal, closest_barycentric, &p);
                apply_shadow(&p, intensity);
            }
        }
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

    //init();
    display(); 

    while (!glfwWindowShouldClose(window)) {

        //idle();    

        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}