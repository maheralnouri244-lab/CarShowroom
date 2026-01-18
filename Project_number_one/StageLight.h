#ifndef STAGELIGHT_H
#define STAGELIGHT_H

#include <GL/glut.h>

class StageLight {
private:
    float baseSize;
    float coneHeight;
    float coneRadius;
    float color[3];
    float rotX, rotY, rotZ;

    float bColor[3];
    float bOpacity;
    float bLength;
    float bRadius;

public:
    StageLight(float baseSize = 2.0f, float coneHeight = 6.0f, float coneRadius = 3.0f,
        float r = 0.0f, float g = 0.0f, float b = 0.0f);
    void setRotation(float angleX, float angleY, float angleZ);

    void updateBeam(float r, float g, float b, float opacity, float length, float radius);

    void draw() const;
};

#endif