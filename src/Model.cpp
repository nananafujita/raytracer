#include "Model.h"

#include <stdio.h>
#include <stdlib.h>

Model::Model(char* f)
: filename(f)
{
}

// Parse OBJ file to define triangles, fills triangles vector
bool Model::loadOBJ()
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file %s. Program exiting.", filename);
        return false;
    }

    char buffer[256];
    // read line by line of the OBJ file. 
    // referenced https://www.cs.cmu.edu/~mbz/personal/graphics/obj.html regarding OBJ files
    std::vector<Vec3> vertices, normals, textures;
    while (fgets(buffer, sizeof(buffer), file))
    {
        char type[3];
        float x, y, z;
        if (sscanf(buffer, "v %f %f %f", &x, &y, &z) == 3) {
            vertices.emplace_back(x, y, z); 
        } else if (sscanf(buffer, "vn %f %f %f", &x, &y, &z) == 3) {
            normals.emplace_back(x, y, z); 
        } else if (sscanf(buffer, "vt %f %f %f", &x, &y, &z) == 2) {
            textures.emplace_back(x, y, -1.0f); 
        } else if (sscanf(buffer, "vt %f %f %f", &x, &y, &z) == 3) {
            textures.emplace_back(x, y, z); 
        } else if (buffer[0] == 'f') {
            char* ptr = buffer + 2;     // move ptr to next char after 'f '
            int v, vt, vn;
            int faceCount = 0;

            std::vector<int> faceV, faceVt, faceVn;
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
                // move ptr to next set of vertex information
                while (*ptr && !isspace(*ptr)) { 
                    ptr++; 
                } 
                while (*ptr && isspace(*ptr)) { 
                    ptr++; 
                }
            } // end while 
            // inconsistent formatting - if vt/vn values exist, should exist for all vertices in face
            if ((faceVt.size() > 0 && faceVt.size() != faceV.size()) || 
                (faceVn.size() > 0 && faceVn.size() != faceV.size())) {
                    printf("Error: inconsistent number of face vertex, texture, and normal indicies. All faces must have the same number of variables. %s\n", buffer);
                    return false;
            }
            if (!addFace(faceV, vertices, faceVn, normals)) {
                printf("Error: error adding face. %s\n", buffer);
                return false;
            }

        } else {
            printf("Error: unrecognized line in OBJ file. %s/n", buffer);
            return false;
        } 
        // end if (parsing buffer in OBJ file)
    } // end while
    return true;
} 

// Function takes in all vertices defining a face; which could be greater than 3 depending on how obj is defined
// Triangulates face using ear clipping 
bool Model::addFace(std::vector<int>& faceVertexIndices, std::vector<Vec3>& vertices, std::vector<int>& faceNormalIndices, std::vector<Vec3>& normals)
{
    // 3 vertex indices defining possible triangle
    int v0 = faceVertexIndices.size() - 1;
    int v1 = 0;
    int v2 = 1;

    int attempts = 0;
    while (faceVertexIndices.size() > 3 && attempts <= faceVertexIndices.size()) {
        attempts++;
        Triangle t;
        if (findTriangle(faceVertexIndices, vertices, faceNormalIndices, normals, t)) {
           attempts = 0;
        }
    }
    if (faceVertexIndices.size() == 3) {

    }
    if (attempts > faceVertexIndices.size()) {
        printf("Error: failed to find valid triangle, too many attempts.");
        return false;
    }
} 

bool Model::findTriangle(std::vector<int>& faceVertexIndices, std::vector<Vec3>& vertices, std::vector<int>& faceNormalIndices, std::vector<Vec3>& normals, Triangle& t)
{
    // find the next convex triangle
    // evaluate triangle abc with b as the potential ear
    for (int a = 0; a < faceVertexIndices.size(); a++) {
        int b = (a + 1) % faceVertexIndices.size();
        int c = (a + 2) % faceVertexIndices.size();

        Point3& pA = vertices[faceVertexIndices[a]];
        Point3& pB = vertices[faceVertexIndices[b]];
        Point3& pC = vertices[faceVertexIndices[c]];

        Vec3 edgeBA = pA - pB;
        Vec3 edgeBC = pC - pB;
        
        if (Vec3::angleBetween(edgeBA, edgeBC) < M_PI) {
            bool validTriangle = true;
            for (int i = 0; i < faceVertexIndices.size(); i++) {
                if (i == a || i == b || i == c) continue;
                Point3& target = vertices[faceVertexIndices[i]];
                if (Triangle::isInside(pA, pB, pC, target)) {
                    validTriangle = false;
                    break;
                }
            }
            if (validTriangle) {
                if (!faceNormalIndices.empty()) {
                    t = Triangle(a, b, c, a, b, c);
                } else {
                    t = Triangle(a, b, c, -1, -1, -1);
                }
                triangles.push_back(t);

                Vec3 normal = Vec3::cross(edgeBA, edgeBC);
                normal.normalize();
                faceNormals.push_back(normal);
                return true;
            }
        }
    }
    
    printf("Error: no valid convex triangle found.");
    return false;
}