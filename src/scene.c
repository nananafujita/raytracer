#include "scene.h"

#include <strings.h>
#include <stdlib.h>


int load_scene(char* filename) 
{
    FILE* file = fopen(filename, "r");
    
    if (!file) {
        parse_error(FILE_OPEN_ERROR, 0);
        return -1;
    }

    int num_objects = 0;
    double ambient_light[3];

    if (fscanf(file, "%i", &num_objects) != 1) {
        parse_error(MISSING_OBJECT_COUNT, 0);
        return -1;
    }

    ParseStatus status;
    status = parse_vector(file, "amb:", ambient_light);
    if (status != PARSE_SUCCESS) {
        parse_error(status, 0);
        return -1;
    }

    Scene scene;
    scene.num_spheres = 0;
    scene.num_triangles = 0;
    scene.num_lights = 0;

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
            parse_error(TYPE_MISMATCH, i+1);
            return -1;
        }

        if (strcasecmp(type, "sphere") == 0) {
            status = parse_sphere(file, &s);
            if (status != PARSE_SUCCESS) {
                parse_error(status, i+1);
                return -1;
            }

            scene.spheres[scene.num_spheres++] = s;
            if (scene.num_spheres > MAX_SPHERES) {
                parse_error(TOO_MANY_OBJECTS, i+1);
                return -1;
            }
        } else if (strcasecmp(type, "triangle") == 0) {
            status = parse_triangle(file, &t);
            if (status != PARSE_SUCCESS) {
                parse_error(status, i+1);
                return -1;
            }

            scene.triangles[scene.num_triangles++] = t;
            if (scene.num_triangles > MAX_TRIANGLES) {
                parse_error(TOO_MANY_OBJECTS, i+1);
                return -1;
            } 
        } else if (strcasecmp(type, "light") == 0) {
            status = parse_light(file, &l);
            if (status!= PARSE_SUCCESS) {
                parse_error(status, i+1);
                return -1;
            }

            scene.lights[scene.num_lights++] = l;
            if (scene.num_lights > MAX_LIGHTS) {
                parse_error(TOO_MANY_OBJECTS, i+1);
                return -1;
            } 
        } else {
            parse_error(UNKNOWN_OBJECT, i+1);
            return -1;
        }
    }

    // the case where ther are more objects than initially declared
    if (fscanf(file, "%s", type) == 1) {
        fprintf(stderr, "Warning: more objects found in file than declared (%i).\n", num_objects);
    }
    return 0;
}

void parse_error(ParseStatus status, int object_number)
{
    if (status != PARSE_SUCCESS && object_number > 0) {
        fprintf(stderr, "Error parsing file on object #%i: ", object_number);
    }
    switch(status) {
        case PARSE_SUCCESS:
            break;
        case FILE_OPEN_ERROR:
            fprintf(stderr, "Unable to open scene file.\n");
            break;
        case MISSING_OBJECT_COUNT:
            fprintf(stderr, "first line must be an integer representing number of objects in the scene.\n");
            break;
        case TYPE_MISMATCH:
            fprintf(stderr, "mismatch in expected and actual type identifiers. Refer to README for scene formatting rules.\n");
            break;
        case MISSING_VALUES:
            fprintf(stderr, "not enough values representing object data. Refer to README for scene formatting rules.\n");
            break;
        case VALUE_OUT_OF_BOUNDS:
            fprintf(stderr, "value is out of bounds. Refer to README for scene formatting rules.\n");
            break;
        case TOO_MANY_OBJECTS:
            fprintf(stderr, "too many objects in file. Consider updating MAX_*OBJECTS in scene.h\n");
            break;
        case UNKNOWN_OBJECT:
            fprintf(stderr, "unknown object name. Refer to README for scene formatting rules.\n");
            break;
        default:
            fprintf(stderr, "Unknown error.\n");
            break;
    }
}

ParseStatus parse_light(FILE* file, Light* light) 
{
    ParseStatus status;
    
    status = parse_vector(file, "pos:", light->position);
    if (status != PARSE_SUCCESS) return status;
    status = parse_vector(file, "col:", light->color);
    if (status != PARSE_SUCCESS) return status;
    status = validate_light(light);
    if (status != PARSE_SUCCESS) return status;

    return PARSE_SUCCESS;
}

ParseStatus parse_sphere(FILE* file, Sphere* sphere)
{
    ParseStatus status;
    status = parse_vector(file, "pos:", sphere->position);
    if (status != PARSE_SUCCESS) return status;

    status = parse_double(file, "rad:", &sphere->radius);
    if (status != PARSE_SUCCESS) return status;

    status = parse_vector(file, "dif:", sphere->color_diffuse);
    if (status != PARSE_SUCCESS) return status;

    status = parse_vector(file, "spe:", sphere->color_specular);
    if (status != PARSE_SUCCESS) return status;

    status = parse_double(file, "shi:", &sphere->shine);
    if (status != PARSE_SUCCESS) return status;

    status = validate_sphere(sphere);
    if (status != PARSE_SUCCESS) return status;

    return PARSE_SUCCESS;
}

ParseStatus parse_triangle(FILE* file, Triangle* triangle)
{
    ParseStatus status;
    for (int v=0; v<3; v++) {
        status = parse_vector(file, "pos:", triangle->vertices[v].position);
        if (status != PARSE_SUCCESS) return status;

        status = parse_vector(file, "nor:", triangle->vertices[v].normal);
        if (status != PARSE_SUCCESS) return status;

        status = parse_vector(file, "dif:", triangle->vertices[v].color_diffuse);
        if (status != PARSE_SUCCESS) return status;

        status = parse_vector(file, "spe:", triangle->vertices[v].color_specular);
        if (status != PARSE_SUCCESS) return status;

        status = parse_double(file, "shi:", &triangle->vertices[v].shine);
        if (status != PARSE_SUCCESS) return status;
    }
    status = validate_triangle(triangle);
    if (status != PARSE_SUCCESS) return status;

    return PARSE_SUCCESS;
}

// Parses 1 double value. 
// Pass file, expected type (eg. "rad:") given file formatting rules (check README), and pointer to value
ParseStatus parse_double(FILE* file, char* type, double* d) 
{
    char str[50];
    fscanf(file, "%s", str);
    ParseStatus status = validate_type(type, str);
    if (status != PARSE_SUCCESS) return status;

    if (fscanf(file, "%lf", d) != 1) return MISSING_VALUES;

    return PARSE_SUCCESS;
}

// Parses values comprised of 3 doubles 
// Pass file, expected type (eg. "rad:") given file formatting rules (check README), and value array
ParseStatus parse_vector(FILE* file, char* type, double v[3])
{
    char str[50];
    fscanf(file, "%s", str);
    ParseStatus status = validate_type(type, str);
    if (status != PARSE_SUCCESS) return status;

    if (fscanf(file, "%lf %lf %lf", &v[0], &v[1], &v[2]) != 3) return MISSING_VALUES;

    return PARSE_SUCCESS;
}

// Checks if the type identifier matches expected 
// This is where formatting errors ("position:" vs "pos:") and missing information is caught
ParseStatus validate_type(char* expected, char* actual)
{
    if (strcasecmp(expected, actual) != 0) return TYPE_MISMATCH;

    return PARSE_SUCCESS;
}

ParseStatus validate_light(Light* light) 
{
    if (light->color[0] < 0 || light->color[0] > 1 || 
        light->color[1] < 0 || light->color[1] > 1 || 
        light->color[2] < 0 || light->color[2] > 1) {
            return VALUE_OUT_OF_BOUNDS;
        }
    return PARSE_SUCCESS;
}

ParseStatus validate_sphere(Sphere* sphere) 
{
    if (sphere->radius < 0 || sphere->shine < 0 || sphere->shine > 1 ||
        sphere->color_diffuse[0] < 0 || sphere->color_diffuse[0] > 1 ||
        sphere->color_diffuse[1] < 0 || sphere->color_diffuse[1] > 1 ||
        sphere->color_diffuse[2] < 0 || sphere->color_diffuse[2] > 1 ||
        sphere->color_specular[0] < 0 || sphere->color_specular[0] > 1 ||
        sphere->color_specular[1] < 0 || sphere->color_specular[1] > 1 ||
        sphere->color_specular[2] < 0 || sphere->color_specular[2] > 1) 
    {
        return VALUE_OUT_OF_BOUNDS;
    }
    return PARSE_SUCCESS;
}

ParseStatus validate_triangle(Triangle* triangle) 
{
    for (int i=0; i<3; i++) {
        if (triangle->vertices[i].shine < 0 || triangle->vertices[i].shine > 1 ||
            triangle->vertices[i].color_diffuse[0] < 0 || triangle->vertices[i].color_diffuse[0] > 1 ||
            triangle->vertices[i].color_diffuse[1] < 0 || triangle->vertices[i].color_diffuse[1] > 1 ||
            triangle->vertices[i].color_diffuse[2] < 0 || triangle->vertices[i].color_diffuse[2] > 1 ||
            triangle->vertices[i].color_specular[0] < 0 || triangle->vertices[i].color_specular[0] > 1 ||
            triangle->vertices[i].color_specular[1] < 0 || triangle->vertices[i].color_specular[1] > 1 ||
            triangle->vertices[i].color_specular[2] < 0 || triangle->vertices[i].color_specular[2] > 1) 
        {
            return VALUE_OUT_OF_BOUNDS;
        }
    }
    return PARSE_SUCCESS;
}