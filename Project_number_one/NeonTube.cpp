#include "NeonTube.h"
#include <math.h>
#define M_PI acos(-1)
using namespace std;
NeonTube::NeonTube(float posX, float posY, float posZ, float len, float w, char ax)
    : x(posX), y(posY), z(posZ), length(len), width(w), axis(ax) {
    r = 0.0f; g = 0.9f; b = 1.0f;
}

void NeonTube::setColor(float red, float green, float blue) {
    r = red; g = green; b = blue;
}

void NeonTube::draw() const {
    glPushMatrix();

    glTranslatef(x, y, z);

    if (axis == 'x' || axis == 'X') {
        glRotatef(90, 0, 1, 0);
        glTranslatef(0, 0, -length / 2.0f);
    }
    else if (axis == 'y' || axis == 'Y') {
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, 0, -length / 2.0f);
    }
    else {
        glTranslatef(0, 0, -length / 2.0f);
    }

    glColor3f(r, g, b);

    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, width, width, length, 32, 1);

    gluDisk(quad, 0, width, 32, 1);
    glPushMatrix();
    glTranslatef(0, 0, length);
    gluDisk(quad, 0, width, 32, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);
    glPopMatrix();
}

void NeonTube::drawC(float ringInnerRadius, float ringOuterRadius) const {
    glColor3f(0, 0.9, 1);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(90.0f, 1, 0, 0);

    glScalef(3.0f, 1.0f, 1.0f);

    glutSolidTorus(ringInnerRadius / 2, ringOuterRadius / 2, 16, 32);

    glPopMatrix();
}
void NeonTube::drawColumns(float ringOuterRadius,
    int columnCount,
    float columnHeight,
    float columnRadius,
    float stretchX,
    float stretchY) const {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(90.0f, 1, 0, 0);

    for (int i = 0; i < columnCount; ++i) {
        float angle = (2.0f * static_cast<float>(M_PI) * i) / static_cast<float>(columnCount);

        // ≈Õœ«ÀÌ«  «·⁄„Êœ ⁄·Ï «·„ÕÌÿ «·»Ì÷ÊÌ
        float px = (ringOuterRadius / 2.0f) * cos(angle) * stretchX;
        float py = (ringOuterRadius / 2.0f) * sin(angle) * stretchY;

        glPushMatrix();
        glTranslatef(px, py, 0.0f);

        glRotatef(180.0f, 1, 0, 0);

        glColor3f(0.05f, 0.05f, 0.10f);
        GLUquadric* quad = gluNewQuadric();
        gluCylinder(quad, columnRadius, columnRadius, columnHeight, 16, 1);
        gluDeleteQuadric(quad);

        glPopMatrix();
    }

    glPopMatrix();
}
