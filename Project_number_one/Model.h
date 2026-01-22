#ifndef MODEL_H
#define MODEL_H

#include <GL/glut.h>
#include <vector>
#include <string>
#include <map>

struct Vec3 { float x, y, z; };
struct Vec2 { float x, y; };

struct SubMesh {
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;
    unsigned int textureID = 0;
    float r = 1.0f, g = 1.0f, b = 1.0f;
};

class Model {
public:
    Model();
    ~Model();

    void load(const char* filename);
    void assignTexture(std::string materialName, unsigned int texID);
    void assignColor(std::string materialName, float r, float g, float b);
    void draw();

private:
    std::map<std::string, SubMesh> meshes;
    std::vector<std::string> meshOrder;

    GLuint displayListID;
    bool isCompiled;
    void compileDisplayList();
};

#endif
