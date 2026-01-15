#ifndef HPILLAR_H
#define HPILLAR_H

#include <GL/glut.h>

class Hpillar {
private:
    float height;
    float width1;
    float width2;
    float x, y, z;
    float r, g, b;

public:
    Hpillar(float xPos,float yPos, float zPos, float h = 5.0f, float w1 = 0.8f,float w2=0.8f);

    void draw() const;

    void setColor(float red, float green, float blue);
};

#endif