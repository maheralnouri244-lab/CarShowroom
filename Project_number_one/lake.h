#pragma once
#include <GL/glut.h>

class Lake {
private:
    float x, y, z;     
    float width, depth;
    float alpha;       

public:
    Lake(float posX, float posY, float posZ,
        float w = 100.0f, float d = 60.0f, float a = 0.5f);

    void drawSurface() const;
    void drawReflection(void (*scene)());
};