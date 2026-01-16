#ifndef NEON_TUBE_H
#define NEON_TUBE_H

#include <GL/glut.h>

class NeonTube {
private:
    float x, y, z;
    float length;
    float width;
    char axis;
    float r, g, b;

public:
    NeonTube(float posX, float posY, float posZ, float len, float w, char ax);
    void setColor(float red, float green, float blue);
    void draw() const;
    void drawC(float ringInnerRadius,float ringOuterRadius) const;

    void drawColumns(float ringOuterRadius,
        int columnCount,
        float columnHeight,
        float columnRadius,
        float stretchX = 1.5f,
        float stretchY = 1.0f) const;
};

#endif