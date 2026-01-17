#include "StreetLamp.h"

StreetLamp::StreetLamp(float height, float radius)
    : poleHeight(height), poleRadius(radius) {

    armCurveRadius = height * 0.5f;

    headLight = new StageLight(radius * 2.0f, radius * 4.0f, radius * 3.0f, 1.0f, 0.9f, 0.5f);

    headLight->setRotation(180.0f, 0.0f, 0.0f);
}

StreetLamp::~StreetLamp() {
    delete headLight;
}

void StreetLamp::drawArmCurve() const {
    const int segments = 16;
    const float angleStep = 90.0f / segments;

    glPushMatrix();
    glTranslatef(0.0f, poleHeight, 0.0f);

    for (int i = 0; i < segments; i++) {
        glColor3f(0.2f, 0.2f, 0.2f);
        GLUquadric* quad = gluNewQuadric();
        gluCylinder(quad, poleRadius * 0.8f, poleRadius * 0.8f, armCurveRadius / segments, 16, 16);
        gluDeleteQuadric(quad);

        glTranslatef(0.0f, 0.0f, armCurveRadius / segments);
        glRotatef(angleStep, 1.0f, 0.0f, 0.0f);
    }

    headLight->draw();

    glPopMatrix();
}

void StreetLamp::draw() const {
    glPushMatrix();

    glColor3f(0.15f, 0.15f, 0.15f);
    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, poleRadius, poleRadius * 0.8f, poleHeight, 16, 16);

    gluDisk(quad, 0.0f, poleRadius * 2.0f, 16, 1);
    glPopMatrix();

    drawArmCurve();

    gluDeleteQuadric(quad);
    glPopMatrix();
}