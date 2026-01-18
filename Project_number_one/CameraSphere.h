#ifndef CAMERASPHERE_H
#define CAMERASPHERE_H

#include <GL/glut.h>

class CameraSphere {
private:
    float cubeSize;
    float sphereRadius; 
    float smallCubeSize;

    float lookX, lookY, lookZ;

public:
    CameraSphere(float cubeSize = 2.0f, float sphereRadius = 1.0f, float smallCubeSize = 0.6f);

    void setLookDirection(float lx, float ly, float lz);

    void draw();
};

#endif