#include "Lake.h"

Lake::Lake(float posX, float posY, float posZ,
    float w, float d, float a)
    : x(posX), y(posY), z(posZ),
    width(w), depth(d), alpha(a) {}

void Lake::drawSurface() const {
    glPushMatrix();
    glTranslatef(x, y, z);

    glColor4f(0.3f, 0.5f, 0.8f, alpha); 
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, 0, -depth / 2);
    glVertex3f(width / 2, 0, -depth / 2);
    glVertex3f(width / 2, 0, depth / 2);
    glVertex3f(-width / 2, 0, depth / 2);
    glEnd();

    glPopMatrix();
}

void Lake::drawReflection(void (*scene)()) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glScalef(1.0f, -1.0f, 1.0f);

    glTranslatef(0.0f, 1, 0.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.4f);

    scene();

    glDisable(GL_BLEND);
    glPopMatrix();
}