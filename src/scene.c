#include "scene.h"

// #include <cstdio>

// Checks if the type identifier matches expected 
// This is where formatting errors ("position:" vs "pos:") and missing information is caught
void validate_type(char* expected, char* actual)
{
    if (strcasecmp(expected, actual) != 0) {
        printf("Error parsing file: expected %s, found %s.\n", expected, actual);
        exit(0);
    }
}

void parse_double(FILE* file, char* type, double d) 
{
    char str[50];
    fscanf(file, "%s", &str);
    validate_type(type, str);
    if (fscanf(file, "%lf", d) != 1) {
        printf("Error parsing file: expected 1 value at %s", str);
        exit(0);
    }
}

// Parses values comprised of 3 doubles 
void parse_vector(FILE* file, char* type, double v[3])
{
    char str[50];
    fscanf(file, "%s", &str);
    validate_type(type, str);
    if (fscanf(file, "%lf %lf %lf", &v[0], &v[1], &v[2]) != 3) {
        printf("Error parsing file: expected 3 values at %s", str);
        exit(0);
    }  
}

void validate_sphere(Sphere s) 
{
    if (s.radius < 0 || s.shine < 0 || s.shine > 1 ||
        s.color_diffuse[0] < 0 || s.color_diffuse[0] > 1 ||
        s.color_diffuse[1] < 0 || s.color_diffuse[1] > 1 ||
        s.color_diffuse[2] < 0 || s.color_diffuse[2] > 1 ||
        s.color_specular[0] < 0 || s.color_specular[0] > 1 ||
        s.color_specular[1] < 0 || s.color_specular[1] > 1 ||
        s.color_specular[2] < 0 || s.color_specular[2] > 1) 
    {
        printf("Error in sphere definition. Refer to README for requirements.\n");
    }
}

int load_scene(char* filename) 
{
    FILE* file = fopen(filename, "r");
    
    if (!file) {
        printf("Unable to open scene file %s. Exiting program.\n", filename);
        return -1;
    }

    int object_count = 0;
    char object_type[50];
    float ambient_light[3];

    if (fscanf(file, "%i", &object_count) != 1) {
        printf("Error reading scene file. First line must be integer representing number of objects in scene.\n");
        return -1;
    }
    parse_vector(file, "amb:", ambient_light);

    char* type[10];
    Sphere s;
    Triangle t;
    Vertex v;

    for (int i=0; i<object_count; i++) {
        fscanf(file, "%s\n", &type);

        if (strcasecmp(type, "sphere") == 0) {
            parse_vector(file, "pos:", s.position);
            parse_double(file, "rad:", s.radius);
            parse_vector(file, "dif:", s.color_diffuse);
            parse_vector(file, "spe:", s.color_specular);
            parse_double(file, "shi:", s.shine);
            validate_sphere(s);
        } else if (strcasecmp(type, "triangle") == 0) {
            parse_vector(file, "pos:", s.position);
            parse_double(file, "rad:", s.radius);
            parse_vector(file, "dif:", s.color_diffuse);
            parse_vector(file, "spe:", s.color_specular);
            parse_double(file, "shi:", s.shine);
        }
    }

}