#include "PlusRing3D.h"

PlusRing3D::PlusRing3D(float armLen, float armThick,
    float ringInner, float ringOuter)
    : armLength(armLen), armThickness(armThick),
    ringInnerRadius(ringInner), ringOuterRadius(ringOuter) {}

void PlusRing3D::draw() const {
    glPushMatrix();

    glColor3f(0.025f, 0.075f, 0.1125f);

    // «·–—«⁄ «·√›ﬁÌ
    glPushMatrix();
    glTranslatef(ringOuterRadius+ armLength/2, 0.0f, 0.0f);
    glScalef(armLength, armThickness, armThickness);
    glutSolidCube(1.0f);
    glPopMatrix();
    // –—«⁄ «›ﬁÌ  «‰Ì
    glPushMatrix();
    glTranslatef(-ringOuterRadius - armLength / 2, 0.0f, 0.0f);
    glScalef(armLength, armThickness, armThickness);
    glutSolidCube(1.0f);
    glPopMatrix();

    // «·–—«⁄ «·‘«ﬁÊ·Ì
    glPushMatrix();
    glTranslatef(0.0f, ringOuterRadius+armLength/2, 0.0f);
    glScalef(armThickness, armLength, armThickness);
    glutSolidCube(1.0f);
    glPopMatrix();

    //–—«⁄ ‘«ﬁÊ·Ì À«‰Ì
    glPushMatrix();
    glTranslatef(0.0f, -ringOuterRadius - armLength / 2, 0.0f);
    glScalef(armThickness, armLength, armThickness);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(0.05f, 0.05f, 0.1f);

    // «·Õ·ﬁ…
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glutSolidTorus(ringInnerRadius, ringOuterRadius, 32, 64);
    glPopMatrix();

    glColor3f(0.025f, 0.075f, 0.1125f);

    // Õ·ﬁ… À«‰Ì…
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glutSolidTorus(ringInnerRadius/2, ringOuterRadius/2, 16, 32);
    glPopMatrix();


    glPopMatrix();
}