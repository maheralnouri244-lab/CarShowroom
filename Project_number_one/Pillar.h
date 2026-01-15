#ifndef PILLAR_H
#define PILLAR_H

#include <GL/glut.h>

class Pillar {
private:
    float height;
    float width;
    float x, y, z;
    float r, g, b;

public:
    Pillar(float xPos,float yPos, float zPos, float h = 5.0f, float w = 0.8f);

    void draw() const;

    void setColor(float red, float green, float blue);
};

#endif