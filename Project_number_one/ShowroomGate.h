#ifndef SHOWROOMGATE_H
#define SHOWROOMGATE_H
#include <GL/glut.h>

class ShowroomGate {
public:
    void draw() const;
    void drawGateArch(float cx = 0.0f, float cz = 0.0f);
};

#endif