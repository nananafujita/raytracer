#ifndef MODEL_H
#define MODEL_H

#include <vector>

class Model 
{
public:
    Model(char* filename);
    bool loadOBJ();
private:
    char* filename;
    std::vector<float> vertices;   //vertices, stored as x, y, z
    std::vector<float> normals;  //normals, stored as x, y, z
    std::vector<float> textures;  //texture coords, stored as u, v
    std::vector<int> faceV;
    std::vector<int> faceVt;
    std::vector<int> faceVn;
};

#endif