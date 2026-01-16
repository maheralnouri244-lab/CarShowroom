#include "ShowroomDoor.h"

ShowroomDoor::ShowroomDoor(float posX, float posY, float posZ,
    float w, float h)
    : x(posX), y(posY), z(posZ),
    width(w), height(h), offsetY(0.0f) {}

void ShowroomDoor::draw() const {
    glPushMatrix();
    glTranslatef(x, y + offsetY, z);

    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glScalef(width, height, 2.0f); 
    glutSolidCube(1.0f);
    glPopMatrix();

    int lineCount = 6; 
    float spacing = height / (lineCount + 1);

    for (int i = 0; i < lineCount; ++i) {
        float yPos = -height / 2.0f + (i + 1) * spacing;

        glPushMatrix();
        glColor3f(0.05f, 0.05f, 0.1f); 
        glTranslatef(-22.5f, yPos, 1.1f); 

        GLUquadric* quad = gluNewQuadric();
        glRotatef(90, 0, 1, 0);
        gluCylinder(quad, 0.5f, 0.5f, width, 16, 1);
        gluDeleteQuadric(quad);

        glPopMatrix();
    }

    glPopMatrix();
}

void ShowroomDoor::moveUp(float step) {
    if (offsetY <30)
    offsetY += step;
}

void ShowroomDoor::moveDown(float step) {
    if (offsetY >0)
    offsetY -= step;
}