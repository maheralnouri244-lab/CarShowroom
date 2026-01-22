#include "Lake.h"
#include <iostream> 

Lake::Lake(float posX, float posY, float posZ, float w, float d, float a)
    : x(posX), y(posY), z(posZ), width(w), depth(d), alpha(a) {
}

void Lake::drawSurface() {
    glPushMatrix();
    glTranslatef(x, y, z);

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.6f, 0.9f, 0.5f);

    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-width / 2, 0, -depth / 2);
    glVertex3f(width / 2, 0, -depth / 2);
    glVertex3f(width / 2, 0, depth / 2);
    glVertex3f(-width / 2, 0, depth / 2);
    glEnd();

    glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Lake::drawReflection(void (*scene)()) {
    glPushMatrix();

    glTranslatef(0, y, 0);
    glScalef(1.0f, -1.0f, 1.0f);
    glTranslatef(0, -y, 0);

    glPushAttrib(GL_FRONT_FACE);
    glFrontFace(GL_CW);

    scene();

    glPopAttrib();
    glPopMatrix();
}
