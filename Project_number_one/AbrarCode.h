#ifndef ABRARCODE_H
#define ABRARCODE_H

#include <GL/glut.h>
#include "BeetleCar.h"

class AbrarCode {
public:
    AbrarCode();
    void draw4Cars();
    void interact();
    void update();
    BeetleCar myCar;
};

#endif
