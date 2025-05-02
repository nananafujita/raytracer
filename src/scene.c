#include "scene.h"

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

void validate_light(Light* l) 
{
    if (l->color[0] < 0 || l->color[0] > 1 || 
        l->color[1] < 0 || l->color[1] > 1 || 
        l->color[2] < 0 || l->color[2] > 1) {
            printf("Error in light definition. Refer to README for requirements.\n");
            exit(0);
        }
}

void validate_sphere(Sphere* s) 
{
    if (s->radius < 0 || s->shine < 0 || s->shine > 1 ||
        s->color_diffuse[0] < 0 || s->color_diffuse[0] > 1 ||
        s->color_diffuse[1] < 0 || s->color_diffuse[1] > 1 ||
        s->color_diffuse[2] < 0 || s->color_diffuse[2] > 1 ||
        s->color_specular[0] < 0 || s->color_specular[0] > 1 ||
        s->color_specular[1] < 0 || s->color_specular[1] > 1 ||
        s->color_specular[2] < 0 || s->color_specular[2] > 1) 
    {
        printf("Error in sphere definition. Refer to README for requirements.\n");
        exit(0);
    }
}

void validate_triangle(Triangle* t) 
{
    for (int i=0; i<3; i++) {
        if (t->vertices[i].shine < 0 || t->vertices[i].shine > 1 ||
            t->vertices[i].color_diffuse[0] < 0 || t->vertices[i].color_diffuse[0] > 1 ||
            t->vertices[i].color_diffuse[1] < 0 || t->vertices[i].color_diffuse[1] > 1 ||
            t->vertices[i].color_diffuse[2] < 0 || t->vertices[i].color_diffuse[2] > 1 ||
            t->vertices[i].color_specular[0] < 0 || t->vertices[i].color_specular[0] > 1 ||
            t->vertices[i].color_specular[1] < 0 || t->vertices[i].color_specular[1] > 1 ||
            t->vertices[i].color_specular[2] < 0 || t->vertices[i].color_specular[2] > 1) 
        {
            printf("Error in triangle definition. Refer to README for requirements.\n");
            exit(0);
        }
    }
}

int load_scene(char* filename) 
{
    FILE* file = fopen(filename, "r");
    
    if (!file) {
        printf("Unable to open scene file %s. Exiting program.\n", filename);
        return -1;
    }

    int num_objects = 0;
    char object_type[50];
    float ambient_light[3];

    if (fscanf(file, "%i", &num_objects) != 1) {
        printf("Error reading scene file. First line must be integer representing number of objects in scene.\n");
        return -1;
    }
    parse_vector(file, "amb:", ambient_light);

    Scene scene;
    scene.num_spheres = 0;
    scene.num_triangles = 0;

    char type[10];
    Sphere s;
    Triangle t;
    Light l;

    for (int i=0; i<num_objects; i++) {
        fscanf(file, "%s\n", &type);

        if (strcasecmp(type, "sphere") == 0) {
            parse_vector(file, "pos:", s.position);
            parse_double(file, "rad:", s.radius);
            parse_vector(file, "dif:", s.color_diffuse);
            parse_vector(file, "spe:", s.color_specular);
            parse_double(file, "shi:", s.shine);
            validate_sphere(&s);
            scene.spheres[scene.num_spheres++] = s;
            if (scene.num_spheres > MAX_SPHERES) {
                printf("Error: too many spheres. Increase MAX_SPHERES.\n");
                exit(0);
            }
        } else if (strcasecmp(type, "triangle") == 0) {
            for (int v=0; v<3; v++) {
                parse_vector(file, "pos:", t.vertices[v].position);
                parse_vector(file, "nor:", t.vertices[v].normal);
                parse_vector(file, "dif:", t.vertices[v].color_diffuse);
                parse_vector(file, "spe:", t.vertices[v].color_specular);
                parse_double(file, "shi:", t.vertices[v].shine);
            }
            validate_triangle(&t);
            scene.triangles[scene.num_triangles++] = t;
            if (scene.num_triangles > MAX_TRIANGLES) {
                printf("Error: too many triangles. Increase MAX_TRIANGLES\n");
                exit(0);
            } 
        } else if (strcasecmp(type, "light") == 0) {
            parse_vector(file, "pos:", l.position);
            parse_vector(file, "col:", l.color);
            validate_light(&l);
            scene.lights[scene.num_lights++] = l;
            if (scene.num_lights > MAX_LIGHTS) {
                printf("Error: too many lights. Increase MAX_LIGHTS\n");
                exit(0);
            } 
        } else {
            printf("Error: unknown type in scene file: %s. Refer to README for acceptable types.\n", type);
            exit(0);
        }
    }
    return 0;
}