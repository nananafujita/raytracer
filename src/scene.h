#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <stdio.h>
#include "math3d.h"

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
    double pos[3];
    double color[3];
} Light;

typedef struct Sphere {
    double pos[3];
    double radius;
    double diffuse[3];
    double specular[3];
    double shininess;
} Sphere;

typedef struct TriangleVertex {
    double pos[3];
    double normal[3];
    double diffuse[3];
    double specular[3];
    double shininess;
} TriangleVertex;

typedef struct Triangle {
    TriangleVertex vertices[3];
} Triangle;

extern Light lights[MAX_LIGHTS];
extern Triangle triangles[MAX_TRIANGLES];
extern Sphere spheres[MAX_SPHERES];
extern double ambient_light[3];
extern int num_lights;
extern int num_triangles;
extern int num_spheres;

int load_scene(char* filename);
void parse_error(ParseStatus status, int object_number);

ParseStatus parse_light(FILE* file, Light* light);
ParseStatus parse_sphere(FILE* file, Sphere* sphere);
ParseStatus parse_triangle(FILE* file, Triangle* triangle);

ParseStatus parse_double(FILE* file, char* type, double* d);
ParseStatus parse_vec3(FILE* file, char* type, double v[3]);
ParseStatus validate_type(char* expected, char* actual);

ParseStatus validate_light(Light* light);
ParseStatus validate_sphere(Sphere* sphere);
ParseStatus validate_triangle(Triangle* triangle);

#endif