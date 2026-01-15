#ifndef PLUSRING3D_H
#define PLUSRING3D_H
#include <GL/glut.h>

class PlusRing3D {
public:
    PlusRing3D(float armLength = 1.0f, float armThickness = 0.3f,
               float ringInnerRadius = 0.4f, float ringOuterRadius = 1.0f);
    void draw() const;

private:
    float armLength;
    float armThickness;
    float ringInnerRadius;
    float ringOuterRadius;
};

#endif