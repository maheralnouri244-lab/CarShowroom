#ifndef GARDEN_H
#define GARDEN_H

#include <GL/glut.h>
#include <cmath>
#include "Lake.h" // تضمين البحيرة

class Garden {
public:
    Garden();

    // كائن البحيرة أصبح جزءاً من الحديقة
    Lake* gardenLake;

    void setTextures(unsigned int pavement, unsigned int grass, unsigned int wall, unsigned int metal);

    // دالة رسم الحديقة (الأرضية، السور، الديكورات - بدون الماء)
    void draw(float x, float y, float z, bool isNight);

    // دوال جديدة للتحكم في الماء والانعكاس من داخل الحديقة
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

    // دوال التجميل
    void drawSimpleTree(float x, float y, float z);
    void drawFlowerBed(float x, float y, float z);
    void drawGazebo(float x, float y, float z);
    void drawWoodenBridge(float x, float y, float z, float length, float width);
    void drawModernBench(float x, float y, float z, float rotation);
    void drawGardenLamp(float x, float y, float z, bool isOn);
    void drawRock(float x, float y, float z, float size);
    void drawReed(float x, float y, float z);
};

#endif
