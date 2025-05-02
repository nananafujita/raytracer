#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <stdio.h>

#define MAX_TRIANGLES 20000
#define MAX_SPHERES 100
#define MAX_LIGHTS 100

typedef struct Vertex {
    double position[3];
    double normal[3];
    double color_diffuse[3];
    double color_specular[3];
    double shine;
} Vertex;

typedef struct Sphere {
    double position[3];
    double radius;
    double color_diffuse[3];
    double color_specular[3];
    double shine;
} Sphere;

typedef struct Triangle {
    Vertex vertices[3];
} Triangle;

typedef struct Scene {
    Triangle triangles[MAX_TRIANGLES];
    Sphere spheres[MAX_SPHERES];
} Scene;

int load_scene(char* filename);
void parse_double(FILE* file, char* type, double d);
void parse_vector(FILE* file, char* type, double p[3]);
void validate_type(char* expected, char* actual);

void validate_sphere(struct Sphere* s);

#endif