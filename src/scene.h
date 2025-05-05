#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <stdio.h>

#define MAX_TRIANGLES 20000
#define MAX_SPHERES 100
#define MAX_LIGHTS 100

typedef enum {
    PARSE_SUCCESS = 0,
    FILE_OPEN_ERROR,
    MISSING_OBJECT_COUNT,
    MISSING_VALUES,
    TYPE_MISMATCH,
    TOO_MANY_OBJECTS,
    UNKNOWN_OBJECT,
    VALUE_OUT_OF_BOUNDS
} ParseStatus;

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
void parse_error(ParseStatus status, int object_number);

ParseStatus parse_light(FILE* file, Light* light);
ParseStatus parse_sphere(FILE* file, Sphere* sphere);
ParseStatus parse_triangle(FILE* file, Triangle* triangle);

ParseStatus parse_double(FILE* file, char* type, double* d);
ParseStatus parse_vector(FILE* file, char* type, double p[3]);
ParseStatus validate_type(char* expected, char* actual);

ParseStatus validate_light(Light* light);
ParseStatus validate_sphere(Sphere* sphere);
ParseStatus validate_triangle(Triangle* triangle);

#endif