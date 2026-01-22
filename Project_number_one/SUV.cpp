#include "SUV.h"

void SUV::draw() {
    glPushMatrix();
    glTranslatef(x, y + 3.2f, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale * 2.0f, scale * 2.0f, scale * 2.0f);
    glDisable(GL_TEXTURE_2D);

    drawChassis(2.4f, 0.9f, 5.5f, r, g, b);
    glPushMatrix(); 
    glTranslatef(0, 0.9f, -0.2f); 
    drawCabin(2.3f, 0.8f, 3.5f, 0.8f, r, g, b); 
    glPopMatrix();

    glRotatef(-rotation, 0, 1, 0);
    float wR = 0.45f; float wW = 0.4f;
    drawWheel(1.1f, -0.45f, 1.8f, wR, wW); 
    drawWheel(-1.5f, -0.45f, 1.8f, wR, wW);
    drawWheel(1.1f, -0.45f, -1.8f, wR, wW); 
    drawWheel(-1.5f, -0.45f, -1.8f, wR, wW);
    glPopMatrix();
}
