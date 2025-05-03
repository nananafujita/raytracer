#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <stdio.h>

#define MAX_TRIANGLES 20000
#define MAX_SPHERES 100
#define MAX_LIGHTS 100

typedef struct Light {
    double position[3];
    double color[3];
} Light;

typedef struct Sphere {
    double position[3];
    double radius;
    double color_diffuse[3];
    double color_specular[3];
    double shine;
} Sphere;

typedef struct TriangleVertex {
    double position[3];
    double normal[3];
    double color_diffuse[3];
    double color_specular[3];
    double shine;
} TriangleVertex;

typedef struct Triangle {
    TriangleVertex vertices[3];
} Triangle;

typedef struct Scene {
    Light lights[MAX_LIGHTS];
    Triangle triangles[MAX_TRIANGLES];
    Sphere spheres[MAX_SPHERES];
    int num_lights;
    int num_triangles;
    int num_spheres;
} Scene;

int load_scene(char* filename);

void parse_double(FILE* file, char* type, double* d);
void parse_vector(FILE* file, char* type, double p[3]);
void validate_type(char* expected, char* actual);

void validate_light(Light* l);
void validate_sphere(Sphere* s);
void validate_triangle(Triangle* t);

#endif