#include "Ambulance.h"

void Ambulance::draw() {
    glPushMatrix();
    glTranslatef(x, y + 3.0f, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale * 2.0f, scale * 2.0f, scale * 2.0f);
    glDisable(GL_TEXTURE_2D);

    drawChassis(2.4f, 1.0f, 2.0f, 0.95f, 0.95f, 0.95f);
    glPushMatrix(); 
    glTranslatef(0, 0.2f, -1.5f); 
    glColor3f(0.95f, 0.95f, 0.95f); 
    glScalef(2.4f, 1.8f, 3.5f); 
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(0, 1.0f, 1.0f); 
    drawCabin(2.2f, 0.6f, 1.5f, 0.7f, 0.95f, 0.95f, 0.95f); 
    glPopMatrix();

    glColor3f(0.9f, 0.0f, 0.0f);
    glPushMatrix(); 
    glTranslatef(1.21f, 0.5f, -1.5f); 
    glScalef(0.1f, 1.0f, 0.3f); 
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(1.21f, 0.5f, -1.5f);
    glScalef(0.1f, 0.3f, 1.0f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(-1.21f, 0.5f, -1.5f);
    glScalef(0.1f, 1.0f, 0.3f);
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(-1.21f, 0.5f, -1.5f);
    glScalef(0.1f, 0.3f, 1.0f);
    glutSolidCube(1); 
    glPopMatrix();

    glRotatef(-rotation, 0, 1, 0);
    float wR = 0.4f; float wW = 0.4f;
    drawWheel(1.1f, -0.5f, 1.5f, wR, wW);
    drawWheel(-1.5f, -0.5f, 1.5f, wR, wW);
    drawWheel(1.1f, -0.5f, -2.0f, wR, wW);
    drawWheel(-1.5f, -0.5f, -2.0f, wR, wW);
    glPopMatrix();
}
