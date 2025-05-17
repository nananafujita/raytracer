#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "scene.h"

#define WIDTH 800
#define HEIGHT 600
#define FOV 60.0
#define EPSILON 1e-6

struct Pixel {
    Vec3 position;
    Vec3 normal;
    Vec3 color;
    Vec3 diffuse;
    Vec3 specular;
    Vec3 barycentric;
    double shininess;
};

void key_callback(GLFWwindow* wind, int key, int scancode, int action, int mods) 
{
    switch(key)
    {
        case 256:
            exit(0);
            break;
    }
} 

void calculate_hit_normal(Sphere s, Vec3 hit_point, Vec3 hit_normal)
{  
    hit_normal = divide(subtract(hit_point, s.position), s.radius);
}

// using analytic method of testing whether the ray and sphere intersect
// direction must be normalized
int hit_sphere(Sphere sphere, Vec3 origin, Vec3 direction, Vec3 hit_point, Vec3 hit_normal)
{
    double a = 1.0;
    double b = 2.0 * dot(direction, subtract(origin, sphere.position));
    double c = dot(subtract(origin, sphere.position), subtract(origin, sphere.position)) - pow(sphere.radius, 2.0);

    // calculate discriminant
    double discriminant = pow(b, 2) - 4.0 * c;
    if (discriminant < 0) return -1;


    double t0 = (-b - sqrt(discriminant)) / 2.0;
    double t1 = (-b + sqrt(discriminant)) / 2.0;

    // sphere is behind ray
    if (t0 <= 0 && t1 <= 0) return -1;

    double t_intersect = min(t0, t1);
    int normal_direction = 1;

    if (t_intersect <= 0) {
        t_intersect = max(t0, t1);
        normal_direction = -1;
    }

    hit_point = add(multiply(t_intersect, direction), origin);
    calculate_hit_normal(sphere, hit_point, hit_normal);
    normalize(&hit_normal);

    if (normal_direction < 0)  hit_normal = multiply(-1.0, hit_normal);
    return 0;
}

int hit_triangle(Triangle triangle, Vec3 origin, Vec3 direction, Vec3* hit_point, Vec3* triangle_normal, Vec3* barycentric) {
    // identify normal of the plane one which the triangle lies
    Vec3 v01 = subtract(triangle.vertices[1].position, triangle.vertices[0].position);
    Vec3 v02 = subtract(triangle.vertices[2].position, triangle.vertices[0].position);
    *triangle_normal = cross(v01, v02);

    double area = length(*triangle_normal) / 2.0;
    normalize(triangle_normal);

    // plane is parallel to camera ray; not visible.
    if (fabs(dot(*triangle_normal, direction)) < EPSILON) return -1;
    
    // definition of a plane: ax+by+cz+d
    // a, b, c defined by hit_normal
    double d = -dot(*triangle_normal, triangle.vertices[0].position);

    // value of t along the ray that hits the plane 
    // if t <= 0, the plane is behind or at the camera; not visible
    double t_intersect = -(dot(*triangle_normal, origin) + d) / dot(*triangle_normal, direction);
    if (t_intersect <= 0) return -1;

    *hit_point = add(multiply(t_intersect, direction), origin);

    // find barycentric coordinates to determine if hit point is inside triangle
    Vec3 v0hP = subtract(*hit_point, triangle.vertices[0].position);

    double dot0101 = dot(v01, v01);
    double dot0102 = dot(v01, v02);
    double dot0202 = dot(v02, v02);
    double dot0hp01 = dot(v0hP, v01);
    double dot0hp02 = dot(v0hP, v02);
    double denominator = dot0101 * dot0202 - dot0102 * dot0102;
    barycentric->x = (dot0202 * dot0hp01 - dot0102 * dot0hp02) / denominator;
    barycentric->y = (dot0101 * dot0hp02 - dot0102 * dot0hp01) / denominator;
    barycentric->z = 1.0 - barycentric->x - barycentric->y;
    if (barycentric->x < -EPSILON || barycentric->y < -EPSILON || barycentric->z < -EPSILON ||
        barycentric->x > 1.0 + EPSILON || barycentric->y > 1.0 + EPSILON || barycentric->z > 1.0 + EPSILON) {
            return -1;
    }
    
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

    struct Pixel closest_object;
    int closest_object_found = -1;
    // loop through camera rays
    for (int i=0; i<WIDTH; i++) {
        for (int j=0; j<HEIGHT; j++) {
            closest_object_found = -1;
            double pixel_x = i * step_width - x;
            double pixel_y = j * step_height - y;
            Vec3 direction = {pixel_x, pixel_y, z};
            normalize(&direction);
            Vec3 hit_point;
            Vec3 hit_normal;
            Vec3 hit_barycentric;
            for (int s=0; s<num_spheres; s++) {
                if (hit_sphere(spheres[s], origin, direction, hit_point, hit_normal) == 0) {
                    if (closest_object_found != 0 || length(hit_point) < length(closest_object.position)) { // camera is at origin, so we can do length() directly
                        closest_object.position = hit_point;
                        closest_object.normal = hit_normal;
                        closest_object.color = ambient_light;
                        closest_object.diffuse = spheres[s].color_diffuse;
                        closest_object.specular = spheres[s].color_specular;
                        closest_object.barycentric = (Vec3) {1.0, 1.0, 1.0};
                        closest_object.shininess = spheres[s].shininess;
                        closest_object_found = 0;
                    }
                }
            }
            for (int t=0; t<num_triangles; t++) {
                if (hit_triangle(triangles[t], origin, direction, &hit_point, &hit_normal, &hit_barycentric) == 0) {
                    if (closest_object_found != 0 || length(hit_point) < length(closest_object.position)) {
                        closest_object.position = hit_point;
                        closest_object.normal = interpolate(hit_barycentric, triangles[t].vertices[0].normal, triangles[t].vertices[1].normal, triangles[t].vertices[2].normal);
                        normalize(&closest_object.normal);
                        closest_object.color = ambient_light;
                        closest_object.diffuse = interpolate(hit_barycentric, triangles[t].vertices[0].color_diffuse, triangles[t].vertices[1].color_diffuse, triangles[t].vertices[2].color_diffuse);
                        closest_object.specular = interpolate(hit_barycentric, triangles[t].vertices[0].color_specular, triangles[t].vertices[1].color_specular, triangles[t].vertices[2].color_specular);
                        closest_object.barycentric = hit_barycentric;
                        closest_object.shininess = (hit_barycentric.z * triangles[t].vertices[0].shininess) + (hit_barycentric.x * triangles[t].vertices[1].shininess) + (hit_barycentric.y * triangles[t].vertices[2].shininess);
                        closest_object_found = 0;
                    }
                }
            }
            printf("%i %i\n", j, i);
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