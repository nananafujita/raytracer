#ifndef RENDER_H
#define RENDER_H

#include "scene.h"
#include "macros.h"

typedef struct Pixel {
    int is_sphere;
    int idx;
    double pos[3];
    double normal[3];
    double color[3];
    double diffuse[3];
    double specular[3];
    double bary[3];
    double shininess;
} Pixel;

void render(Vec3* intensities);
int hit_sphere(int idx, double origin[3], double direction[3], double hit_point[3], double hit_normal[3]);
int hit_triangle(int idx, double origin[3], double direction[3], double hit_point[3], double triangle_normal[3], double barycentric[3]);
void define_pixel(int is_sphere, int index, double hit_point[3], double hit_normal[3], double hit_barycentric[3], Pixel* pixel);
void apply_shadow(Pixel* p, double intensity[3]);
void calculate_intensity(Light* l, Pixel* p, double light_dir[3], double intensity[3]);

#endif