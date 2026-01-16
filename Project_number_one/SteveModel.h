#ifndef STEVEMODEL_H
#define STEVEMODEL_H

#include <GL/glut.h>

class SteveModel {
private:
    float unit;

    static float SkinColor[3];
    static float HairColor[3];
    static float ShirtColor[3];
    static float PantsColor[3];
    static float ShoeColor[3];

    void drawBox(float width, float height, float depth);

public:
    SteveModel(float baseUnit = 0.1f);

    static void setSkinColor(float r, float g, float b);
    static void setHairColor(float r, float g, float b);
    static void setShirtColor(float r, float g, float b);
    static void setPantsColor(float r, float g, float b);
    static void setShoeColor(float r, float g, float b);

    void draw();
    void drawHead();
    void drawTorso();
    void drawArms();
    void drawLegs();
};

#endif
