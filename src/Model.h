#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Vec3.h"
#include "Triangle.h"

class Model 
{
public:
    Model(char* filename);
    bool loadOBJ();
    const std::vector<Triangle>& getTriangles() const { return triangles; }
private:
    bool addFace(std::vector<int>& faceVertexIndices, std::vector<Vec3>& vertices, std::vector<int>& faceNormalIndices, std::vector<Vec3>& normals);
    bool findTriangle(std::vector<int>& faceVertexIndices, std::vector<Vec3>& vertices, int indices[3], Vec3& normal, Triangle& t); 
    char* filename;

    std::vector<Triangle> triangles;
    std::vector<Vec3> faceNormals; // triangulated face normals - facenormals[i] is for triangles[i]
};

#endif