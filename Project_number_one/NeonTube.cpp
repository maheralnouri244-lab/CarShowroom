#include "NeonTube.h"

NeonTube::NeonTube(float posX, float posY, float posZ, float len, float w, char ax)
    : x(posX), y(posY), z(posZ), length(len), width(w), axis(ax) {
    r = 0.0f; g = 0.9f; b = 1.0f;
}

void NeonTube::setColor(float red, float green, float blue) {
    r = red; g = green; b = blue;
}

void NeonTube::draw() const {
    glPushMatrix();

    glTranslatef(x, y, z);

    if (axis == 'x' || axis == 'X') {
        glRotatef(90, 0, 1, 0);
        glTranslatef(0, 0, -length / 2.0f);
    }
    else if (axis == 'y' || axis == 'Y') {
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, 0, -length / 2.0f);
    }
    else {
        glTranslatef(0, 0, -length / 2.0f);
    }

    glColor3f(r, g, b);

    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, width, width, length, 32, 1);

    gluDisk(quad, 0, width, 32, 1);
    glPushMatrix();
    glTranslatef(0, 0, length);
    gluDisk(quad, 0, width, 32, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);
    glPopMatrix();
}

void NeonTube::drawC(float ringInnerRadius,float ringOuterRadius) const {
    
    glColor3f(0, 0.9, 1);
    glPushMatrix();
    glTranslatef(x, 0, 0);
    glRotatef(90.0f, 1, 0, 0);
    glutSolidTorus(ringInnerRadius / 2, ringOuterRadius / 2, 16, 32);
    glPopMatrix();
}