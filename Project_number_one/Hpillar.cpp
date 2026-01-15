#include "Hpillar.h"

Hpillar::Hpillar(float xPos,float yPos, float zPos, float h, float w1,float w2) {
    x = xPos;
    z = zPos;
    y = yPos;
    y += h / 2;
    height = h;
    width1 = w1;
    width2 = w2;
    r = 0.05f; g = 0.05f; b = 0.1f;
}

void Hpillar::setColor(float red, float green, float blue) {
    r = red; g = green; b = blue;
}

void Hpillar::draw() const {
    glPushMatrix();
    glTranslatef(x, y, z);

    glColor3f(r, g, b);

    glScalef(width1, height, width2);

    glutSolidCube(1.0f);
    glPopMatrix();
}