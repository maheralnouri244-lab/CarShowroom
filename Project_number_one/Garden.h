#ifndef GARDEN_H
#define GARDEN_H

#include <GL/glut.h>
#include <cmath>
#include "Lake.h"

class Garden {
public:
    Garden();

    Lake* gardenLake;

    void setTextures(unsigned int pavement, unsigned int grass, unsigned int wall, unsigned int metal);

    void draw(float x, float y, float z, bool isNight);

    void drawWaterSurface(float x, float y, float z);
    void drawWaterReflection(float x, float y, float z, void (*scene)());

private:
    unsigned int pavementTex;
    unsigned int grassTex;
    unsigned int wallTex;
    unsigned int metalTex;

    void drawJuiceStall(float x, float y, float z);
    void drawFence(float w, float l);
    void drawMainGate();

    void drawFlowerBed(float x, float y, float z);
    void drawGazebo(float x, float y, float z);
    void drawModernBench(float x, float y, float z, float rotation);
    void drawGardenLamp(float x, float y, float z, bool isOn);
};

#endif
