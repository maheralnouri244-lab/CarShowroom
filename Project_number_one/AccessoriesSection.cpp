#include "AccessoriesSection.h"
#include <cmath>

AccessoriesSection::AccessoriesSection() {}

void AccessoriesSection::drawRim(float scale) {
    glPushMatrix();
    glScalef(scale, scale, scale);

    // ?????? ??????? (????)
    glColor3f(0.9f, 0.9f, 0.95f);
    glutSolidTorus(0.1, 0.5, 10, 20);

    // ?????? (Spokes)
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glRotatef(72 * i, 0, 0, 1);
        glTranslatef(0, 0.25, 0);
        glScalef(0.1, 0.5, 0.05);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glPopMatrix();
}

void AccessoriesSection::drawTire(float scale) {
    glPushMatrix();
    glScalef(scale, scale, scale);
    glColor3f(0.15f, 0.15f, 0.15f); // ???? ????
    glutSolidTorus(0.2, 0.6, 10, 20);

    // ???? ?????? (?????)
    glColor3f(0.1f, 0.1f, 0.1f);
    glutWireTorus(0.201, 0.6, 8, 12);
    glPopMatrix();
}

void AccessoriesSection::drawRimDisplay(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // ???? ??????
    glColor3f(0.5f, 0.35f, 0.2f);
    glPushMatrix();
    glTranslatef(0, 5, 0);
    glScalef(15.0f, 10.0f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ??? ??????
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(-5.0f + (i * 5.0f), 6.0f, 1.5f);
        drawRim(3.0f);
        glPopMatrix();
    }
    glPopMatrix();
}

void AccessoriesSection::drawTireStack(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(-90, 1, 0, 0); // ??? ???????? ?????

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(0, 0, 1.0f + (i * 1.5f));
        drawTire(3.0f);
        glPopMatrix();
    }
    glPopMatrix();
}

void AccessoriesSection::drawOilShelf(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // ???????
    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 6, 0);
    glScalef(10.0f, 12.0f, 3.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ??? ????? (?????? ?????)
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 4; col++) {
            glPushMatrix();
            float colOffset = -3.0f + (col * 2.0f);
            float rowOffset = 3.0f + (row * 3.0f);
            glTranslatef(colOffset, rowOffset, 1.5f);

            if (col % 2 == 0) glColor3f(1.0f, 0.8f, 0.0f); // Shell/Yellow
            else glColor3f(1.0f, 0.0f, 0.0f); // Red line

            glScalef(1.0f, 1.5f, 0.8f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }
    glPopMatrix();
}

void AccessoriesSection::drawCounter(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    // ????? ??????
    glColor3f(0.9f, 0.9f, 0.9f);
    glPushMatrix();
    glTranslatef(0, 3, 0);
    glScalef(15.0f, 6.0f, 4.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ?????? (?????)
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0, 6.2f, 0);
    glScalef(3.0f, 0.2f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
    // ????
    glPushMatrix();
    glTranslatef(0, 7.5f, -1.0f);
    glRotatef(-10, 1, 0, 0);
    glScalef(3.0f, 2.5f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();
}

void AccessoriesSection::drawStairs(float x, float z, float height, int steps) {
    // ??? ??? ???? ?????? ?????? ??????
    float stepHeight = height / steps;
    float stepDepth = 3.0f;
    float stepWidth = 15.0f;

    glColor3f(0.7f, 0.7f, 0.75f); // ??????/?????

    for (int i = 0; i < steps; i++) {
        glPushMatrix();
        // ?? ???? ????? ?????? ?????? ??????
        glTranslatef(x, (i * stepHeight) + (stepHeight / 2), z + (i * stepDepth));
        glScalef(stepWidth, stepHeight, stepDepth);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // ??????? ?????
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    float totalLen = steps * stepDepth;
    glTranslatef(x - (stepWidth / 2), height / 2 + 2, z + totalLen / 2 - stepDepth / 2);
    glRotatef(-30, 1, 0, 0); // ????? ??????
    glScalef(0.5f, 0.5f, totalLen * 1.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void AccessoriesSection::drawRailings(float minX, float maxX, float z, float height) {
    // ??? ????? ?????? ??????
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.5f, 0.7f, 1.0f, 0.3f); // ???? ???? ????
    glPushMatrix();
    float width = maxX - minX;
    float centerX = (maxX + minX) / 2.0f;
    glTranslatef(centerX, height + 5.0f, z); // ?????? ????? 10 (???? 5)
    glScalef(width, 10.0f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ????? ?????? ?????
    glColor3f(0.8f, 0.8f, 0.8f);
    for (float x = minX; x <= maxX; x += 30.0f) {
        glPushMatrix();
        glTranslatef(x, height + 5.0f, z);
        glScalef(1.0f, 10.0f, 1.0f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    glDisable(GL_BLEND);
}

void AccessoriesSection::drawFloor(float minX, float maxX, float startZ, float endZ, float height) {
    // ????? ?????? ??????
    glColor3f(0.85f, 0.85f, 0.9f); // ???? ???? ????

    float width = maxX - minX;
    float depth = endZ - startZ;
    float centerX = (maxX + minX) / 2.0f;
    float centerZ = (startZ + endZ) / 2.0f;

    glPushMatrix();
    glTranslatef(centerX, height - 1.0f, centerZ); // -1 ????? ????? ??? height
    glScalef(width, 2.0f, depth); // ??? ??????? 2
    glutSolidCube(1.0f);
    glPopMatrix();

    drawRailings(minX, maxX, endZ, height); // ??? ??? ?????? ????????
}

void AccessoriesSection::draw(float minX, float maxX, float minZ, float maxZ) {
    // ???? ??????: ?? ????? (minZ side)
    // ?????: ???? ??? ??????
    // ?????: ????? 100 ???? ?? ?????

    float floorHeight = 30.0f;
    float floorDepth = 100.0f;

    // ??? ??????? (???????)
    // ???? ?? minX ??? maxX? ??? minZ ??? minZ + 100
    drawFloor(minX, maxX, minZ, minZ + floorDepth, floorHeight);

    // ??? ????? (???? ?? ????? ??? ??????)
    // ???? ??? ?????? ??????
    drawStairs(minX + 40.0f, minZ + floorDepth, floorHeight, 15);

    // --- ??? ??????? ?????? ---

    // 1. ??? ?????? (??? ??????)
    drawRimDisplay(maxX - 50.0f, floorHeight, minZ + 20.0f);
    drawRimDisplay(maxX - 90.0f, floorHeight, minZ + 20.0f);

    // 2. ?????? ???????? (?? ????? ??????)
    drawCounter(0.0f, floorHeight, minZ + 30.0f);

    // 3. ???? ?????? (??? ??????? ??? ?????)
    drawOilShelf(minX + 100.0f, floorHeight, minZ + 20.0f);

    // 4. ????? ?????? (???????)
    drawTireStack(maxX - 30.0f, floorHeight, minZ + 60.0f);
    drawTireStack(maxX - 60.0f, floorHeight, minZ + 60.0f);

    // 5. ????? ??????? ?????
    // (???? ??????? ??? Furniture ??? ?? ????? ??? ????? ????? ????? ???????? ??????)
    glColor3f(1, 1, 1);
    glPushMatrix(); glTranslatef(-50, floorHeight + 4, minZ + 60);
    glRotatef(-90, 1, 0, 0); glutSolidCone(5, 0.5, 10, 2);
    glPopMatrix();
    glColor3f(0.5, 0.5, 0.5);
    glPushMatrix(); glTranslatef(-50, floorHeight, minZ + 60);
    glRotatef(-90, 1, 0, 0);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.5, 0.5, 4.0, 10, 2);
    gluDeleteQuadric(quad);
    glPopMatrix();
}
