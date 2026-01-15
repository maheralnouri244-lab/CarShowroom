#include "ShowroomGate.h"

void ShowroomGate::draw() const {
    float r = 0.05f, g = 0.05f, b = 0.1f;
    float width = 16.0f;
    float height = 30.0f;
    float depth = 1.0f;
    float cx = 0.0f, cy = 15.0f, cz = 50.0f;
    float entryHeight = 10.0f;

    glColor3f(r, g, b);

    glPushMatrix();
    glTranslatef(cx - width / 2 + 2.0f, entryHeight / 2, cz);
    glScalef(2.0f, entryHeight, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx + width / 2 - 2.0f, entryHeight / 2, cz);
    glScalef(2.0f, entryHeight, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx, (entryHeight + (height - entryHeight) / 2), cz);
    glScalef(width, height - entryHeight, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(r + 0.1f, g + 0.1f, b + 0.1f);
    glPushMatrix();
    glTranslatef(cx, height + 1.0f, cz);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.2, width / 2, 16, 32);
    glPopMatrix();
}