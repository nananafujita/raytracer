#include "Model.h"

#include <stdio.h>
#include <stdlib.h>

Model::Model(char* f)
: filename(f)
{
}

bool Model::loadOBJ()
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file %s. Program exiting.", filename);
        return false;
    }

    char buffer[256];
    // read in line by line of the OBJ file. 
    // for f, different possible input types and number of verices per face 
    // referenced https://www.cs.cmu.edu/~mbz/personal/graphics/obj.html regarding OBJ files
    while (fgets(buffer, sizeof(buffer), file))
    {
        char type[3];
        float x, y, z;
        if (sscanf(buffer, "v %f %f %f", &x, &y, &z) == 3) {
            vertices.push_back(x);      vertices.push_back(y);      vertices.push_back(z); 
        } else if (sscanf(buffer, "vn %f %f %f", &x, &y, &z) == 3) {
            normals.push_back(x);       normals.push_back(y);       normals.push_back(z);
        } else if (sscanf(buffer, "vt %f %f %f", &x, &y, &z) == 2) {
            textures.push_back(x);      textures.push_back(y);
        } else if (sscanf(buffer, "vt %f %f %f", &x, &y, &z) == 3) {
            textures.push_back(x);      textures.push_back(y);      textures.push_back(z);
        } else if (buffer[0] == 'f') {
            char* ptr = buffer + 2;
            int v, vt, vn;
            int faceCount = 0;
            while (*ptr) {
                faceCount++;
                if (sscanf(ptr, "%d/%d/%d", &v, &vt, &vn) == 3){
                    faceV.push_back(v--);     faceVt.push_back(vt--);       faceVn.push_back(vn--);
                } else if (sscanf(ptr, "%d//%d", &v, &vn) == 2){
                    faceV.push_back(v--);                                   faceVn.push_back(vn--);
                } else if (sscanf(ptr, "%d/%d", &v, &vt) == 2) {
                    faceV.push_back(v--);     faceVt.push_back(vt--);
                } else if (sscanf(ptr, "%d", &v)) {
                    faceV.push_back(v--);
                } else {
                    printf("Error: unrecognized line in OBJ file. %s\n", buffer);
                    return false;
                }
                while (*ptr && !isspace(*ptr)) { 
                    ptr++; 
                } 
                while (*ptr && isspace(*ptr)) { 
                    ptr++; 
                }
            }
            if ((faceVt.size() > 0 && faceVt.size() != faceV.size()) || 
                (faceVn.size() > 0 && faceVn.size() != faceV.size())) {
                    printf("Error: inconsistent number of face vertex, texture, and normal indicies. All faces must have the same number of variables. %s\n", buffer);
                    return false;
            }
        } else {
            printf("Error: unrecognized line in OBJ file. %s/n", buffer);
            return false;
        }
    }
    return true;
} 