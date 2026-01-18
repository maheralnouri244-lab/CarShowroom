#ifndef STREETLAMP_H
#define STREETLAMP_H

#include "StageLight.h"
#include <GL/glut.h>
#include <cmath>

class StreetLamp {
private:
    float poleHeight;
    float poleRadius;
    float armCurveRadius;
    float lampColor[3];
    StageLight* headLight;


    void drawArmCurve(bool on) const;

public:
    StreetLamp(float height = 5.0f, float radius = 0.15f);
    ~StreetLamp();

    void setLightColor(float r, float g, float b);
    void draw(bool on) const;
};

#endif