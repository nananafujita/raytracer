#include "render.h"

#define EPSILON 1e-6

// using analytic method of testing whether the ray and sphere intersect
// direction must be normalized
int hit_sphere(Sphere sphere, double origin[3], double direction[3], double hit_point[3], double hit_normal[3])
{
    double origin_sphere[3];
    subtract(origin, sphere.position, origin_sphere);

    double a = 1.0;
    double b = 2.0 * dot(direction, origin_sphere);
    double c = dot(origin_sphere, origin_sphere) - pow(sphere.radius, 2.0);

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
    multiply(direction, t_intersect, hit_point);
    add(origin, hit_point, hit_point);

    subtract(hit_point, sphere.position, hit_normal);
    normalize(hit_normal);

    if (normal_direction < 0)  multiply(hit_normal, -1.0, hit_normal);
    return 0;
}

// Ray-Plane intersection using the plane equation: ax_by+cz+d=0
int hit_triangle(Triangle triangle, double origin[3], double direction[3], double hit_point[3], double triangle_normal[3], double barycentric[3]) {
    // identify normal of the plane one which the triangle lies
    double v01[3]; 
    subtract(triangle.vertices[1].position, triangle.vertices[0].position, v01);
    double v02[3]; 
    subtract(triangle.vertices[2].position, triangle.vertices[0].position, v02);
    cross(v01, v02, triangle_normal);

    double area = length(triangle_normal) / 2.0;
    normalize(triangle_normal);

    // plane is parallel to camera ray; not visible.
    if (fabs(dot(triangle_normal, direction)) < EPSILON) return -1;
    
    // definition of a plane: ax+by+cz+d
    // a, b, c defined by hit_normal
    double d = -dot(triangle_normal, triangle.vertices[0].position);

    // value of t along the ray that hits the plane 
    // if t <= 0, the plane is behind or at the camera; not visible
    double t_intersect = -(dot(triangle_normal, origin) + d) / dot(triangle_normal, direction);
    if (t_intersect <= 0) return -1;

    multiply(direction, t_intersect, hit_point);
    add(origin, hit_point, hit_point);

    // find barycentric coordinates to determine if hit point is inside triangle
    double v0hP[3];
    subtract(hit_point, triangle.vertices[0].position, v0hP);

    double dot0101 = dot(v01, v01);
    double dot0102 = dot(v01, v02);
    double dot0202 = dot(v02, v02);
    double dot0hp01 = dot(v0hP, v01);
    double dot0hp02 = dot(v0hP, v02);
    double denominator = dot0101 * dot0202 - dot0102 * dot0102;
    barycentric[0] = (dot0202 * dot0hp01 - dot0102 * dot0hp02) / denominator;
    barycentric[1] = (dot0101 * dot0hp02 - dot0102 * dot0hp01) / denominator;
    barycentric[2] = 1.0 - barycentric[0] - barycentric[1];
    if (barycentric[0] < -EPSILON || barycentric[1] < -EPSILON || barycentric[2] < -EPSILON ||
        barycentric[0] > 1.0 + EPSILON || barycentric[1] > 1.0 + EPSILON || barycentric[2] > 1.0 + EPSILON) {
            return -1;
    }
    
    return 0;
}

void closest_pixel(int is_sphere, int index, double hit_point[3], double hit_normal[3], double hit_barycentric[3], struct Pixel* pixel)
{
    pixel->position[0] = hit_point[0];      pixel->position[1] = hit_point[1];      pixel->position[2] = hit_point[2];
    pixel->color[0] = ambient_light[0];        pixel->color[1] = ambient_light[1];        pixel->color[2] = ambient_light[2];
    if (is_sphere == 0) {
        for (int i=0; i<3; i++) {
            pixel->normal[i] = hit_normal[i];
            pixel->diffuse[i] = spheres[index].color_diffuse[i];
            pixel->specular[i] = spheres[index].color_specular[i];
            pixel->barycentric[i] = -1.0;
        }
        pixel->shininess = spheres[index].shininess;
    } else {
        interpolate(hit_barycentric, triangles[index].vertices[0].normal, triangles[index].vertices[1].normal, triangles[index].vertices[2].normal, pixel->normal);
        normalize(pixel->normal);
        interpolate(hit_barycentric, triangles[index].vertices[0].color_diffuse, triangles[index].vertices[1].color_diffuse, triangles[index].vertices[2].color_diffuse, pixel->diffuse);
        interpolate(hit_barycentric, triangles[index].vertices[0].color_specular, triangles[index].vertices[1].color_specular, triangles[index].vertices[2].color_specular, pixel->specular);

        pixel->barycentric[0] = hit_barycentric[0]; 
        pixel->barycentric[1] = hit_barycentric[1]; 
        pixel->barycentric[2] = hit_barycentric[2]; 

        pixel->shininess = (hit_barycentric[2] * triangles[index].vertices[0].shininess) + (hit_barycentric[0] * triangles[index].vertices[1].shininess) + (hit_barycentric[1] * triangles[index].vertices[2].shininess);
        // check the index numbers
    }
}

void apply_shadow()
{}
