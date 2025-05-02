#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <stdio.h>


struct Sphere {
    double position[3];
    double radius;
    double color_diffuse[3];
    double color_specular[3];
    double shine;
};

int load_scene(char* filename);
void parse_double(FILE* file, char* type, double d);
void parse_vector(FILE* file, char* type, double p[3]);
void validate_type(char* expected, char* actual);

void validate_sphere(struct Sphere s);

#endif