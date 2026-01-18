#include "OfficeDesk.h"

OfficeDesk::OfficeDesk(float w, float h, float d) : width(w), height(h), depth(d) {
    topThickness = 2.0f;
    legRadius = 1.5f;
    woodColor[0] = 0.4f; woodColor[1] = 0.2f; woodColor[2] = 0.1f; woodColor[3] = 1.0f;
    legColor[0] = 0.3f; legColor[1] = 0.3f; legColor[2] = 0.3f; legColor[3] = 1.0f;
}

void OfficeDesk::draw() {
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    drawLegs();

    glTranslatef(0.0f, height, 0.0f);
    drawTop();

    glPopAttrib();
    glPopMatrix();
}

void OfficeDesk::drawTop() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, woodColor);
    glPushMatrix();
    glScalef(width, topThickness, depth);
    glutSolidCube(1.0);
    glPopMatrix();
}

void OfficeDesk::drawLegs() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, legColor);
    float offsetW = (width / 2.0f) - 3.0f;
    float offsetD = (depth / 2.0f) - 3.0f;
    float legPositions[4][2] = {
        {offsetW, offsetD}, {-offsetW, offsetD},
        {offsetW, -offsetD}, {-offsetW, -offsetD}
    };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legPositions[i][0], 0.0f, legPositions[i][1]);
        glRotatef(-90, 1, 0, 0);
        GLUquadric* q = gluNewQuadric();
        gluCylinder(q, legRadius, legRadius, height, 20, 1);
        gluDeleteQuadric(q);
        glPopMatrix();
    }
}