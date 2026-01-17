#ifndef PERSISTENT_CLOUD_H
#define PERSISTENT_CLOUD_H

#include <GL/glut.h>
#include <vector>

struct CloudBall {
    float x, y, z;
    float radius;
    float grayShade;
};

class PersistentCloud {
private:
    std::vector<CloudBall> balls;
    float posX, posY, posZ;
    float cloudScale;

public:
    PersistentCloud();
    void draw() const;
    void regenerate();
};

#endif#pragma once
