#include "Pillar.h"

Pillar::Pillar(float xPos,float yPos, float zPos, float h, float w) {
    x = xPos;
    z = zPos;
    height = h;
    width = w;
    y = yPos + height / 2.0f;
    r = 0.05f; g = 0.05f; b = 0.1f;
}

void Pillar::setColor(float red, float green, float blue) {
    r = red; g = green; b = blue;
}

void Pillar::draw() const {
    glPushMatrix(); 
    glTranslatef(x, y, z);

    glColor3f(r, g, b);

    glScalef(width, height, width);

    glutSolidCube(1.0f);
    glPopMatrix();
}