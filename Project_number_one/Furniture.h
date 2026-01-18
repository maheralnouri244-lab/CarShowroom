#pragma once
#ifndef FURNITURE_H
#define FURNITURE_H
#include <GL/glut.h>

class Furniture {
public:
    Furniture();
    void drawTableSet(float x, float y, float z);
private:
    void drawChair();
    void drawTable();
    void drawCylinder(float radius, float height);
};
#endif
