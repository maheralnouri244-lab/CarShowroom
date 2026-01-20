#ifndef LAKE_H
#define LAKE_H
#include <GL/glut.h>

class Lake {
public:
    float x, y, z, width, depth, alpha;

    Lake(float posX, float posY, float posZ, float w, float d, float a);
    void drawSurface();
    void drawReflection(void (*scene)());
};
#endif