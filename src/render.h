#ifndef RENDER_H
#define RENDER_H

#include "scene.h"

typedef struct Pixel {
    double position[3];
    double normal[3];
    double color[3];
    double diffuse[3];
    double specular[3];
    double barycentric[3];
    double shininess;
} Pixel;

int hit_sphere(Sphere sphere, double origin[3], double direction[3], double hit_point[3], double hit_normal[3]);
int hit_triangle(Triangle triangle, double origin[3], double direction[3], double hit_point[3], double triangle_normal[3], double barycentric[3]);
void closest_pixel(int is_sphere, int index, double hit_point[3], double hit_normal[3], double hit_barycentric[3], Pixel* pixel);
void apply_shadow();

#endif