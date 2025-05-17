#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <stdio.h>
#include "vec3.h"

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
    Vec3 position;
    Vec3 color;
} Light;

typedef struct Sphere {
    Vec3 position;
    double radius;
    Vec3 color_diffuse;
    Vec3 color_specular;
    double shininess;
} Sphere;

typedef struct TriangleVertex {
    Vec3 position;
    Vec3 normal;
    Vec3 color_diffuse;
    Vec3 color_specular;
    double shininess;
} TriangleVertex;

typedef struct Triangle {
    TriangleVertex vertices[3];
} Triangle;

extern Light lights[MAX_LIGHTS];
extern Triangle triangles[MAX_TRIANGLES];
extern Sphere spheres[MAX_SPHERES];
extern Vec3 ambient_light;
extern int num_lights;
extern int num_triangles;
extern int num_spheres;

int load_scene(char* filename);
void parse_error(ParseStatus status, int object_number);

ParseStatus parse_light(FILE* file, Light* light);
ParseStatus parse_sphere(FILE* file, Sphere* sphere);
ParseStatus parse_triangle(FILE* file, Triangle* triangle);

ParseStatus parse_double(FILE* file, char* type, double* d);
ParseStatus parse_vec3(FILE* file, char* type, Vec3 v);
ParseStatus validate_type(char* expected, char* actual);

ParseStatus validate_light(Light* light);
ParseStatus validate_sphere(Sphere* sphere);
ParseStatus validate_triangle(Triangle* triangle);

#endif