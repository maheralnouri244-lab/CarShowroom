#include "StageLight.h"

StageLight::StageLight(float bSize, float cHeight, float cRadius,
    float r, float g, float b)
    : baseSize(bSize), coneHeight(cHeight), coneRadius(cRadius),
    rotX(0.0f), rotY(0.0f), rotZ(0.0f) {
    color[0] = r; color[1] = g; color[2] = b;
}

void StageLight::setRotation(float angleX, float angleY, float angleZ) {
    rotX = angleX;
    rotY = angleY;
    rotZ = angleZ;
}

void StageLight::draw() const {
    glPushMatrix();

    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

    glColor3f(color[0], color[1], color[2]);

    glPushMatrix();
    glScalef(baseSize, baseSize, baseSize * 3);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -coneHeight / 2 - baseSize / 3, -baseSize * 2);
    glColor3f(0.1f, 0.1f, 0.1f);
    glutSolidCone(coneRadius, coneHeight, 32, 32);

    glColor3f(1.0f, 1.0f, 1.0f);
    GLUquadric* quad = gluNewQuadric();
    glTranslatef(0.0f, 0.0f, -0.1f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quad, 0.0f, coneRadius, 32, 1);
    gluDeleteQuadric(quad);
    glPopMatrix();
    glPopMatrix();
}