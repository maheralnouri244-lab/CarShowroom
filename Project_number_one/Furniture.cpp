#include "Furniture.h"
#include <cmath>

Furniture::Furniture() {}

void Furniture::drawCylinder(float radius, float height) {
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix(); glRotatef(-90, 1, 0, 0); gluCylinder(quad, radius, radius, height, 16, 1); gluDeleteQuadric(quad); glPopMatrix();
    glPushMatrix(); glTranslatef(0, height, 0); glRotatef(-90, 1, 0, 0); GLUquadric* disk = gluNewQuadric(); gluDisk(disk, 0, radius, 16, 1); gluDeleteQuadric(disk); glPopMatrix();
}

void Furniture::drawChair() {
    glColor3f(0.4f, 0.2f, 0.1f);
    float legH = 6.0f; float legW = 0.6f;
    for (float i = -1; i <= 1; i += 2) {
        for (float j = -1; j <= 1; j += 2) {
            glPushMatrix(); glTranslatef(i * 2.0f, legH / 2, j * 2.0f); glScalef(legW, legH, legW); glutSolidCube(1.0f); glPopMatrix();
        }
    }
    glPushMatrix(); glTranslatef(0, legH, 0); glScalef(5.0f, 0.4f, 5.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0, legH + 3.0f, -2.2f); glScalef(5.0f, 6.0f, 0.4f); glutSolidCube(1.0f); glPopMatrix();
}

void Furniture::drawTable() {
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix(); glTranslatef(0, 0, 0); drawCylinder(1.0f, 8.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.2f, 0); glScalef(8.0f, 0.4f, 8.0f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.9f, 0.9f, 0.9f);
    glPushMatrix(); glTranslatef(0, 8.0f, 0); glRotatef(-90, 1, 0, 0);
    GLUquadric* top = gluNewQuadric(); gluDisk(top, 0, 10.0f, 20, 1); gluDeleteQuadric(top);
    glPopMatrix();
}

void Furniture::drawTableSet(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    drawTable();
    float dist = 8.0f;
    glPushMatrix(); glTranslatef(0, 0, -dist); drawChair(); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0, dist); glRotatef(180, 0, 1, 0); drawChair(); glPopMatrix();
    glPushMatrix(); glTranslatef(-dist, 0, 0); glRotatef(90, 0, 1, 0); drawChair(); glPopMatrix();
    glPushMatrix(); glTranslatef(dist, 0, 0); glRotatef(-90, 0, 1, 0); drawChair(); glPopMatrix();
    glPopMatrix();
}
