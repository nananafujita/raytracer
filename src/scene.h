#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include "vec3.h"
#include "hittable.h"

class Scene {
private: 
    static constexpr int max_triangles = 20000;
    static constexpr int max_spheres = 100;
    static constexpr int max_lights = 100;

    enum class ParseStatus {
        PARSE_SUCCESS = 0,
        FILE_OPEN_ERROR,
        MISSING_OBJECT_COUNT,
        MISSING_VALUES,
        TYPE_MISMATCH,
        TOO_MANY_OBJECTS,
        UNKNOWN_OBJECT,
        VALUE_OUT_OF_BOUNDS
    };

    Scene(); 

    void parse_error(ParseStatus status, int object_number);

    ParseStatus parse_light(FILE* file, Light& light);
    ParseStatus parse_sphere(FILE* file, Sphere& sphere);
    ParseStatus parse_triangle(FILE* file, Triangle& triangle);

    ParseStatus parse_double(FILE* file, char* type, double& d);
    ParseStatus parse_vec3(FILE* file, char* type, Vec3 v);
    ParseStatus validate_type(char* expected, char* actual);

    ParseStatus validate_light(Light& light);
    ParseStatus validate_sphere(Sphere& sphere);
    ParseStatus validate_triangle(Triangle& triangle);

public: 
    static Scene& instance();
    bool load_scene(char* filename);

    static Light lights[max_lights];
    Triangle triangles[max_triangles];
    Sphere spheres[max_spheres];
    Vec3 ambient_light;
    static int num_lights;
    static int num_triangles;
    static int num_spheres;
};

#endif