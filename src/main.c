#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "scene.h"

#define WIDTH 800
#define HEIGHT 600
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

void calculate_intersection_normal(Sphere s, Vec3 intersection_point, Vec3 intersection_normal)
{  
    intersection_normal = divide(subtract(intersection_point, s.position), s.radius);
}

// using analytic method of testing whether the ray and sphere intersect
// direction must be normalized
int hit_sphere(Sphere s, Vec3 origin, Vec3 direction, Vec3 intersection_point, Vec3 intersection_normal)
{
    double a = 1.0;
    double b = 2.0 * dot(direction, subtract(origin, s.position));
    double c = dot(subtract(origin, s.position), subtract(origin, s.position)) - pow(s.radius, 2.0);

    // calculate discriminant
    double discriminant = pow(b, 2) - 4.0 * c;
    if (discriminant < 0) return -1;
    double t0 = (-b - sqrt(discriminant)) / 2.0;
    double t1 = (-b + sqrt(discriminant)) / 2.0;

    // sphere is behind ray
    if (t0 <= 0 && t1 <= 0) return -1;

    double t_intersect = min(t0, t1);
    double normal_direction = 1.0;

    if (t_intersect <= 0) {
        t_intersect = max(t0, t1);
        normal_direction = -1.0;
    }

    intersection_point = add(multiply(t_intersect, direction), origin);
    calculate_intersection_normal(s, intersection_point, intersection_normal);

    if (normal_direction < 0)  intersection_normal = multiply(-1.0, intersection_normal);
    return 0;
}

void display()
{
    Vec3 origin = {0, 0, 0};
    double half_angle = (FOV / 2) * M_PI / 180.0;                   // angle (in radians) between center and top of image plane
    double x = (float) WIDTH / (float) HEIGHT * tan(half_angle);    // half width of image plane
    double y = tan(half_angle);                                     // half height of image plane
    double z = -1.0;
    double step_width = 2 * x / WIDTH;                              // step width on the image plane
    double step_height = 2 * y / HEIGHT;                                

    // loop through camera rays
    for (int i=0; i<WIDTH; i++) {
        for (int j=0; j<HEIGHT; j++) {
            double pixel_x = i * step_width - x;
            double pixel_y = j * step_height - y;
            Vec3 direction = {pixel_x, pixel_y, z};
            normalize(&direction);
            Vec3 intersection_point;
            Vec3 intersection_normal;
            for (int s=0; s<num_spheres; s++) {
                if (hit_sphere(spheres[s], origin, direction, intersection_point, intersection_normal) == 0) {
                    printf("hit sphere\n");
                }
            }
            //for (int t=0; t<num_triangles; t++) {
                
            //}
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

    while (!glfwWindowShouldClose(window)) {
       display(); 
        //idle();    

        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}