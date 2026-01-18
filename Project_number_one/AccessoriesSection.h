#ifndef ACCESSORIESSECTION_H
#define ACCESSORIESSECTION_H

#include <GL/glut.h>

class AccessoriesSection {
public:
    AccessoriesSection();
    void draw(float minX, float maxX, float minZ, float maxZ);

private:
    void drawFloor(float minX, float maxX, float startZ, float endZ, float height);
    void drawStairs(float x, float z, float height, int steps);
    void drawRailings(float minX, float maxX, float z, float height);

    // عناصر المعرض
    void drawRimDisplay(float x, float y, float z);
    void drawTireStack(float x, float y, float z);
    void drawOilShelf(float x, float y, float z);
    void drawCounter(float x, float y, float z);

    // أشكال مساعدة
    void drawRim(float scale);
    void drawTire(float scale);
};

#endif
