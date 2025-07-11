#include "scene.h"

#include <strings.h>
#include <stdlib.h>

Scene::Scene()
{
}

Scene& Scene::instance()
{
    static Scene s;
    return s;
}

bool Scene::load_scene(char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        parse_error(ParseStatus::FILE_OPEN_ERROR, 0);
        return false;
    }

    int num_objects = 0;
    if (fscanf(file, "%i", &num_objects) != 1) {
        parse_error(ParseStatus::MISSING_OBJECT_COUNT, 0);
        return false;
    }

    ParseStatus status;
    status = parse_vec3(file, "amb:", ambient_light);
    if (status != ParseStatus::PARSE_SUCCESS) {
        parse_error(status, 0);
        return false;
    }

    Scene::num_lights = 0;
    Scene::num_triangles = 0;
    Scene::num_spheres = 0;

    char type[10];
    Sphere s;
    Triangle t;
    Light l;
    for (int i=0; i<num_objects; i++) {
        int count = fscanf(file, "%s\n", type);
        if (count == EOF) {
            printf("Warning: declared number of objects (%i) and actual (%i) are not the same. Continuing program with %i objects.\n", num_objects, i, i);
            break;
        } else if (count != 1) {
            parse_error(ParseStatus::TYPE_MISMATCH, i+1);
            return false;
        }

        if (strcasecmp(type, "sphere") == 0) {
            status = parse_sphere(file, s);
            if (status != ParseStatus::PARSE_SUCCESS) {
                parse_error(status, i+1);
                return false;
            }
            spheres[num_spheres++] = s;
            if (num_spheres > max_spheres) {
                parse_error(ParseStatus::TOO_MANY_OBJECTS, i+1);
                return false;
            }
        } else if (strcasecmp(type, "triangle") == 0) {
            status = parse_triangle(file, t);
            if (status != ParseStatus::PARSE_SUCCESS) {
                parse_error(status, i+1);
                return false;
            }
            triangles[num_triangles++] = t;
            if (num_triangles > max_triangles) {
                parse_error(ParseStatus::TOO_MANY_OBJECTS, i+1);
                return false;
            } 
        } else if (strcasecmp(type, "light") == 0) {
            status = parse_light(file, l);
            if (status!= ParseStatus::PARSE_SUCCESS) {
                parse_error(status, i+1);
                return false;
            }
            Scene::lights[num_lights++] = l;
            if (num_lights > max_lights) {
                parse_error(ParseStatus::TOO_MANY_OBJECTS, i+1);
                return false;
            } 
        } else {
            parse_error(ParseStatus::UNKNOWN_OBJECT, i+1);
            return false;
        }
    }

    // the case where there are more objects than initially declared
    if (fscanf(file, "%s", type) == 1) {
        fprintf(stderr, "Warning: more objects found in file than declared (%i).\n", num_objects);
    }
    return true;
}

void Scene::parse_error(ParseStatus status, int idx)
{
    if (status != ParseStatus::PARSE_SUCCESS && idx > 0) {
        fprintf(stderr, "Error parsing file on object #%i: ", idx);
    }
    switch(status) {
        case ParseStatus::PARSE_SUCCESS:
            break;
        case ParseStatus::FILE_OPEN_ERROR:
            fprintf(stderr, "Unable to open  file.\n");
            break;
        case ParseStatus::MISSING_OBJECT_COUNT:
            fprintf(stderr, "first line must be an integer representing number of objects in the .\n");
            break;
        case ParseStatus::TYPE_MISMATCH:
            fprintf(stderr, "mismatch in expected and actual type identifiers. Refer to README for scene formatting rules.\n");
            break;
        case ParseStatus::MISSING_VALUES:
            fprintf(stderr, "not enough values representing object data. Refer to README for scene formatting rules.\n");
            break;
        case ParseStatus::VALUE_OUT_OF_BOUNDS:
            fprintf(stderr, "value is out of bounds. Refer to README for scene formatting rules.\n");
            break;
        case ParseStatus::TOO_MANY_OBJECTS:
            fprintf(stderr, "too many objects in file. Consider updating MAX_*OBJECTS in scene.c\n");
            break;
        case ParseStatus::UNKNOWN_OBJECT:
            fprintf(stderr, "unknown object name. Refer to README for  formatting rules.\n");
            break;
        default:
            fprintf(stderr, "Unknown error.\n");
            break;
    }
}

Scene::ParseStatus Scene::parse_light(FILE* file, Light& light) 
{
    ParseStatus status;
    status = parse_vec3(file, "pos:", light.pos);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    status = parse_vec3(file, "col:", light.color);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    status = validate_light(light);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    return ParseStatus::PARSE_SUCCESS;
}

Scene::ParseStatus Scene::parse_sphere(FILE* file, Sphere& sphere)
{
    ParseStatus status;
    status = parse_vec3(file, "pos:", sphere.center);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    status = parse_double(file, "rad:", sphere.radius);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    status = parse_vec3(file, "dif:", sphere.diffuse);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    status = parse_vec3(file, "spe:", sphere.specular);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    status = parse_double(file, "shi:", sphere.shine);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    status = validate_sphere(sphere);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    return ParseStatus::PARSE_SUCCESS;
}

Scene::ParseStatus Scene::parse_triangle(FILE* file, Triangle& triangle)
{
    ParseStatus status;
    for (int i=0; i<3; i++) {
        status = parse_vec3(file, "pos:", triangle.v[i].pos);
        if (status != ParseStatus::PARSE_SUCCESS) return status;

        status = parse_vec3(file, "nor:", triangle.v[i].normal);
        if (status != ParseStatus::PARSE_SUCCESS) return status;

        status = parse_vec3(file, "dif:", triangle.v[i].diffuse);
        if (status != ParseStatus::PARSE_SUCCESS) return status;

        status = parse_vec3(file, "spe:", triangle.v[i].specular);
        if (status != ParseStatus::PARSE_SUCCESS) return status;

        status = parse_double(file, "shi:", triangle.v[i].shine);
        if (status != ParseStatus::PARSE_SUCCESS) return status;
    }
    status = validate_triangle(triangle);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    return ParseStatus::PARSE_SUCCESS;
}

// Parses 1 double value. 
// Pass file, expected type (eg. "rad:") given file formatting rules (check README), and pointer to value
Scene::ParseStatus Scene::parse_double(FILE* file, char* type, double& d) 
{
    char str[50];
    fscanf(file, "%s", str);
    ParseStatus status = validate_type(type, str);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    if (fscanf(file, "%lf", d) != 1) return ParseStatus::MISSING_VALUES;

    return ParseStatus::PARSE_SUCCESS;
}

// Parses values comprised of 3 doubles 
// Pass file, expected type (eg. "rad:") given file formatting rules (check README), and value array
Scene::ParseStatus Scene::parse_vec3(FILE* file, char* type, Vec3 v)
{
    char str[50];
    fscanf(file, "%s", str);
    ParseStatus status = validate_type(type, str);
    if (status != ParseStatus::PARSE_SUCCESS) return status;

    if (fscanf(file, "%lf %lf %lf", &v.x, &v.y, &v.z) != 3) return ParseStatus::MISSING_VALUES;

    return ParseStatus::PARSE_SUCCESS;
}

// Checks if the type identifier matches expected 
// This is where formatting errors ("position:" vs "pos:") and missing information is caught
Scene::ParseStatus Scene::validate_type(char* expected, char* actual)
{
    if (strcasecmp(expected, actual) != 0) return ParseStatus::TYPE_MISMATCH;

    return ParseStatus::PARSE_SUCCESS;
}

Scene::ParseStatus Scene::validate_light(Light& light) 
{
    if (light.color.x < 0 || light.color.x > 1 || 
        light.color.y < 0 || light.color.y > 1 || 
        light.color.z < 0 || light.color.z > 1) {
            return ParseStatus::VALUE_OUT_OF_BOUNDS;
        }
    return ParseStatus::PARSE_SUCCESS;
}

Scene::ParseStatus Scene::validate_sphere(Sphere& sphere) 
{
    if (sphere.radius < 0 ||
        sphere.diffuse.x < 0 || sphere.diffuse.x > 1 ||
        sphere.diffuse.y < 0 || sphere.diffuse.y > 1 ||
        sphere.diffuse.z < 0 || sphere.diffuse.z > 1 ||
        sphere.specular.x < 0 || sphere.specular.x > 1 ||
        sphere.specular.y < 0 || sphere.specular.y > 1 ||
        sphere.specular.z < 0 || sphere.specular.z > 1) 
    {
        return ParseStatus::VALUE_OUT_OF_BOUNDS;
    }
    return ParseStatus::PARSE_SUCCESS;
}

Scene::ParseStatus Scene::validate_triangle(Triangle& triangle) 
{
    for (int i=0; i<3; i++) {
        if (triangle.v[i].diffuse.x < 0 || triangle.v[i].diffuse.x > 1 ||
            triangle.v[i].diffuse.y < 0 || triangle.v[i].diffuse.y > 1 ||
            triangle.v[i].diffuse.z < 0 || triangle.v[i].diffuse.z > 1 ||
            triangle.v[i].specular.x < 0 || triangle.v[i].specular.x > 1 ||
            triangle.v[i].specular.y < 0 || triangle.v[i].specular.y > 1 ||
            triangle.v[i].specular.z < 0 || triangle.v[i].specular.z > 1) 
        {
            return ParseStatus::VALUE_OUT_OF_BOUNDS;
        }
    }
    return ParseStatus::PARSE_SUCCESS;
}