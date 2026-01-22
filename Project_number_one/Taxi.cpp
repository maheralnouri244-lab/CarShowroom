#include "Taxi.h"

void Taxi::draw() {
    glPushMatrix();
    glTranslatef(x, y + 2.5f, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale * 2.0f, scale * 2.0f, scale * 2.0f);
    glDisable(GL_TEXTURE_2D);

    drawChassis(2.2f, 0.7f, 5.0f, 1.0f, 0.8f, 0.0f);
    glPushMatrix();
    glTranslatef(0, 0.7f, 0);
    drawCabin(2.0f, 0.6f, 3.0f, 0.6f, 1.0f, 0.8f, 0.0f);
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix(); 
    glTranslatef(0, 1.4f, 0);
    glScalef(0.8f, 0.2f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glRotatef(-rotation, 0, 1, 0);
    float wR = 0.35f; float wW = 0.3f;
    drawWheel(1.0f, -0.35f, 1.5f, wR, wW);
    drawWheel(-1.3f, -0.35f, 1.5f, wR, wW);
    drawWheel(1.0f, -0.35f, -1.5f, wR, wW);
    drawWheel(-1.3f, -0.35f, -1.5f, wR, wW);
    glPopMatrix();
}