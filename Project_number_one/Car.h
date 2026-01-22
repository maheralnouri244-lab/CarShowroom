#ifndef CAR_H
#define CAR_H

#include <GL/glut.h>
#include <cmath>

class Car {
protected:
    float x, y, z;
    float rotation;
    float r, g, b;
    float scale;

    void drawWheel(float dx, float dy, float dz, float radius, float width);

    void drawChassis(float w, float h, float l, float r, float g, float b);
    void drawCabin(float w, float h, float l, float topRatio, float r, float g, float b);

public:
    Car(float _x, float _y, float _z, float _rot, float _r, float _g, float _b, float _scale = 1.0f);
    virtual ~Car() {}
    virtual void draw() = 0;
};
#endif
