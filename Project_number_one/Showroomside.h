#ifndef SHOWROOMSIDE_H
#define SHOWROOMSIDE_H

#include <GL/glut.h>

class Showroomside {
private:
    float MNX, MXX, MNZ, MXZ, widthx, widthy, widthz, r, g, b, height, num,num2;

public:
    Showroomside(float MAXX, float MINX, float MAXZ, float MINZ, float h = 25, float wx = 0.8f, float wy = 0.8f, float wz = 0.8, int n = 12,int n2=3);

    void draw() const;

    void setColor(float red, float green, float blue);
};

#endif