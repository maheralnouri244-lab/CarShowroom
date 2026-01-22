#include "SportsCar.h"

void SportsCar::draw() {
    glPushMatrix();
    glTranslatef(x, y + 2.0f, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale * 2.0f, scale * 1.6f, scale * 2.0f);
    glDisable(GL_TEXTURE_2D);

    drawChassis(2.4f, 0.6f, 5.2f, r, g, b);
    glPushMatrix(); 
    glTranslatef(0, 0.6f, -0.2f); 
    drawCabin(2.0f, 0.5f, 2.5f, 0.5f, r, g, b); 
    glPopMatrix();

    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix(); 
    glTranslatef(0, 0.8f, -2.4f);
    glScalef(2.2f, 0.1f, 0.5f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(0.8f, 0.6f, -2.4f); 
    glScalef(0.1f, 0.4f, 0.2f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(-0.8f, 0.6f, -2.4f); 
    glScalef(0.1f, 0.4f, 0.2f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    glRotatef(-rotation, 0, 1, 0);
    float wR = 0.38f; 
    float wW = 0.45f;
    drawWheel(1.2f, -0.3f, 1.7f, wR, wW);
    drawWheel(-1.6f, -0.3f, 1.7f, wR, wW);
    drawWheel(1.2f, -0.3f, -1.7f, wR, wW); 
    drawWheel(-1.6f, -0.3f, -1.7f, wR, wW);
    glPopMatrix();
}