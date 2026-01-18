#ifndef OFFICE_DESK_H
#define OFFICE_DESK_H

#include <GL/glut.h>

class OfficeDesk {
private:
    float width, height, depth;
    float legRadius, topThickness;
    GLfloat woodColor[4];
    GLfloat legColor[4];

public:
    OfficeDesk(float w = 80.0f, float h = 30.0f, float d = 45.0f);
    void draw();

private:
    void drawTop();
    void drawLegs();
};

#endif