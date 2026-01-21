#include "Garden.h"
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Garden::Garden() {
    pavementTex = 0; grassTex = 0; wallTex = 0; metalTex = 0;
    gardenLake = new Lake(0, 0.06f, 0, 200.0f, 300.0f, 0.5f);
}

void Garden::drawWaterSurface(float x, float y, float z) {
    gardenLake->x = x;
    gardenLake->z = z;
    gardenLake->drawSurface();
}

void Garden::drawWaterReflection(float x, float y, float z, void (*scene)()) {
    gardenLake->x = x;
    gardenLake->z = z;
    gardenLake->drawReflection(scene);
}

void Garden::setTextures(unsigned int pavement, unsigned int grass, unsigned int wall, unsigned int metal) {
    pavementTex = pavement; grassTex = grass; wallTex = wall; metalTex = metal;
}

void setupGardenTexture() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void Garden::drawSimpleTree(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.4f, 0.26f, 0.13f); glPushMatrix(); glRotatef(-90, 1, 0, 0); glutSolidCone(2, 15, 10, 5); glPopMatrix();
    glColor3f(0.0f, 0.5f, 0.0f); glPushMatrix(); glTranslatef(0, 8, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(7, 12, 10, 5); glPopMatrix();
    glColor3f(0.0f, 0.6f, 0.0f); glPushMatrix(); glTranslatef(0, 12, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(6, 10, 10, 5); glPopMatrix();
    glColor3f(0.1f, 0.8f, 0.1f); glPushMatrix(); glTranslatef(0, 16, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(4, 8, 10, 5); glPopMatrix();
    glPopMatrix();
}

void Garden::drawFlowerBed(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.6f, 0.6f, 0.6f); glPushMatrix(); glScalef(10, 2, 10); glutSolidCube(1); glPopMatrix();
    glColor3f(0.3f, 0.2f, 0.1f); glPushMatrix(); glTranslatef(0, 1.1f, 0); glScalef(9, 0.5, 9); glutSolidCube(1); glPopMatrix();
    for (int i = 0; i < 5; i++) { glPushMatrix(); float fx = (rand() % 8) - 4; float fz = (rand() % 8) - 4; glTranslatef(fx, 2.0f, fz); if (i % 2 == 0) glColor3f(1, 0, 0); else glColor3f(1, 1, 0); glutSolidSphere(0.5, 5, 5); glPopMatrix(); }
    glPopMatrix();
}

void Garden::drawGazebo(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.5f, 0.35f, 0.2f); glPushMatrix(); glScalef(30, 1, 30); glutSolidCube(1); glPopMatrix();
    glColor3f(0.4f, 0.2f, 0.1f); float pOff = 13.0f;
    for (int i = 0; i < 4; i++) { glPushMatrix(); float px = (i < 2) ? pOff : -pOff; float pz = (i % 2 == 0) ? pOff : -pOff; glTranslatef(px, 10, pz); glScalef(1, 20, 1); glutSolidCube(1); glPopMatrix(); }
    glColor3f(0.3f, 0.1f, 0.0f); glPushMatrix(); glTranslatef(0, 20, 0); glRotatef(-45, 0, 1, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(22, 10, 4, 1); glPopMatrix();
    glPopMatrix();
}

void Garden::drawWoodenBridge(float x, float y, float z, float length, float width) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.55f, 0.4f, 0.25f); int numPlanks = 30; float plankW = length / numPlanks;
    for (int i = 0; i < numPlanks; i++) {
        float progress = (float)i / numPlanks; float arcH = sin(progress * M_PI) * 5.0f;
        glPushMatrix(); glTranslatef(-length / 2 + i * plankW, arcH, 0); glScalef(plankW * 0.9, 0.5, width); glutSolidCube(1); glPopMatrix();
        glColor3f(0.3f, 0.2f, 0.1f);
        glPushMatrix(); glTranslatef(-length / 2 + i * plankW, arcH + 3, width / 2 - 0.5); glScalef(0.5, 6, 0.5); glutSolidCube(1); glPopMatrix();
        glPushMatrix(); glTranslatef(-length / 2 + i * plankW, arcH + 3, -width / 2 + 0.5); glScalef(0.5, 6, 0.5); glutSolidCube(1); glPopMatrix();
        glColor3f(0.55f, 0.4f, 0.25f);
    } glPopMatrix();
}

void Garden::drawModernBench(float x, float y, float z, float rotation) {
    glPushMatrix(); glTranslatef(x, y, z); glRotatef(rotation, 0, 1, 0); glDisable(GL_TEXTURE_2D);
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix(); glTranslatef(6, 2, 0); glScalef(1, 4, 6); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(-6, 2, 0); glScalef(1, 4, 6); glutSolidCube(1); glPopMatrix();
    glColor3f(0.6f, 0.3f, 0.1f); glPushMatrix(); glTranslatef(0, 4.5f, 0); glScalef(14, 1, 6); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 7.5f, -2.5f); glScalef(14, 4, 1); glutSolidCube(1); glPopMatrix();
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix(); glTranslatef(5, 6, -2.5f); glScalef(0.5, 4, 0.5); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(-5, 6, -2.5f); glScalef(0.5, 4, 0.5); glutSolidCube(1); glPopMatrix();
    glPopMatrix();
}

void Garden::drawGardenLamp(float x, float y, float z, bool isOn) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.1f, 0.1f, 0.1f); glPushMatrix(); glScalef(1, 15, 1); glutSolidCube(1); glPopMatrix();
    glTranslatef(0, 8, 0); glColor3f(0.2f, 0.2f, 0.2f); glutSolidSphere(1.5, 10, 10);
    if (isOn) {
        glDisable(GL_LIGHTING); glColor3f(1.0f, 1.0f, 0.8f); glutSolidSphere(1.6, 10, 10);
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glDepthMask(GL_FALSE);
        glPushMatrix(); glRotatef(90, 1, 0, 0); glColor4f(1.0f, 0.9f, 0.5f, 0.2f); glutSolidCone(10.0, 20.0, 20, 10); glPopMatrix();
        glDepthMask(GL_TRUE); glDisable(GL_BLEND); glEnable(GL_LIGHTING);
    }
    else { glColor3f(0.4f, 0.4f, 0.4f); glutSolidSphere(1.6, 10, 10); }
    glPopMatrix();
}

void Garden::drawRock(float x, float y, float z, float size) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.5f, 0.5f, 0.55f); glScalef(size, size * 0.7, size); glRotatef(rand() % 360, 0, 1, 0); glRotatef(rand() % 90, 1, 0, 0); glutSolidDodecahedron(); glPopMatrix();
}

void Garden::drawReed(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D); glColor3f(0.2f, 0.4f, 0.1f);
    for (int i = 0; i < 5; i++) { glPushMatrix(); glTranslatef((rand() % 10) / 10.0f, 0, (rand() % 10) / 10.0f); glScalef(0.05f, 1.5f + (rand() % 10) / 10.0f, 0.05f); glutSolidCube(1); glPopMatrix(); }
    glColor3f(0.4f, 0.2f, 0.1f); glTranslatef(0, 1.5f, 0); glScalef(0.15f, 0.6f, 0.15f); glutSolidSphere(1, 5, 5); glPopMatrix();
}

void Garden::drawJuiceStall(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 0.5f, 0.0f); glPushMatrix(); glTranslatef(0, 5, 0); glScalef(12, 10, 12); glutSolidCube(1); glPopMatrix();
    glColor3f(0.0f, 0.8f, 0.0f); glPushMatrix(); glTranslatef(0, 11, 6); glScalef(10, 2, 1); glutSolidCube(1); glPopMatrix();
    glColor3f(1, 1, 0); glPushMatrix(); glTranslatef(-3, 6, 6.5); glutSolidTeapot(1); glPopMatrix();
    glColor3f(1, 0, 1); glPushMatrix(); glTranslatef(3, 6, 6.5); glutSolidTeapot(1); glPopMatrix();
    glPopMatrix();
}

void Garden::drawFence(float w, float l) {
    glDisable(GL_TEXTURE_2D); glColor3f(0.2f, 0.2f, 0.2f); float poleH = 8.0f; float gap = 10.0f;
    for (float i = -w / 2; i <= w / 2; i += gap) {
        glPushMatrix(); glTranslatef(i, poleH / 2, -l / 2); glScalef(1, poleH, 1); glutSolidCube(1); glPopMatrix();
        glPushMatrix(); glTranslatef(i, poleH / 2, l / 2); glScalef(1, poleH, 1); glutSolidCube(1); glPopMatrix();
    }
    for (float i = -l / 2; i <= l / 2; i += gap) {
        glPushMatrix(); glTranslatef(-w / 2, poleH / 2, i); glScalef(1, poleH, 1); glutSolidCube(1); glPopMatrix();
        if (abs(i) > 20) {
            glPushMatrix(); glTranslatef(w / 2, poleH / 2, i); glScalef(1, poleH, 1); glutSolidCube(1); glPopMatrix();
        }
    }
}

void Garden::drawMainGate() {
    glDisable(GL_TEXTURE_2D); glColor3f(0.8f, 0.7f, 0.6f);
    glPushMatrix(); glTranslatef(30, 10, 0); glScalef(10, 20, 10); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(-30, 10, 0); glScalef(10, 20, 10); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 18, 0); glScalef(70, 4, 8); glutSolidCube(1); glPopMatrix();
    glColor3f(0.2f, 0.6f, 0.2f); glPushMatrix(); glTranslatef(0, 22, 0); glScalef(40, 4, 2); glutSolidCube(1); glPopMatrix();
}

void Garden::draw(float x, float y, float z, bool isNight) {
    glPushMatrix();
    glTranslatef(x, y, z); 

    float gardenW = 350.0f; float gardenL = 500.0f;
    float lakeW = 200.0f; float lakeL = 300.0f;

    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, pavementTex); setupGardenTexture();
    glColor3f(0.9f, 0.9f, 0.9f); float tS = 0.02f;
    glBegin(GL_QUADS); glNormal3f(0, 1, 0);
    float topH = (gardenL - lakeL) / 2.0f; float topZ = -lakeL / 2 - topH / 2;
    glTexCoord2f(0, 0); glVertex3f(-gardenW / 2, 0.05f, topZ - topH / 2); glTexCoord2f(1, 0); glVertex3f(gardenW / 2, 0.05f, topZ - topH / 2);
    glTexCoord2f(1, 1); glVertex3f(gardenW / 2, 0.05f, topZ + topH / 2); glTexCoord2f(0, 1); glVertex3f(-gardenW / 2, 0.05f, topZ + topH / 2);
    float botZ = lakeL / 2 + topH / 2;
    glTexCoord2f(0, 0); glVertex3f(-gardenW / 2, 0.05f, botZ - topH / 2); glTexCoord2f(1, 0); glVertex3f(gardenW / 2, 0.05f, botZ - topH / 2);
    glTexCoord2f(1, 1); glVertex3f(gardenW / 2, 0.05f, botZ + topH / 2); glTexCoord2f(0, 1); glVertex3f(-gardenW / 2, 0.05f, botZ + topH / 2);
    float sideW = (gardenW - lakeW) / 2.0f;
    glTexCoord2f(0, 0); glVertex3f(-lakeW / 2 - sideW, 0.05f, -lakeL / 2); glTexCoord2f(1, 0); glVertex3f(-lakeW / 2, 0.05f, -lakeL / 2);
    glTexCoord2f(1, 1); glVertex3f(-lakeW / 2, 0.05f, lakeL / 2); glTexCoord2f(0, 1); glVertex3f(-lakeW / 2 - sideW, 0.05f, lakeL / 2);
    glTexCoord2f(0, 0); glVertex3f(lakeW / 2, 0.05f, -lakeL / 2); glTexCoord2f(1, 0); glVertex3f(lakeW / 2 + sideW, 0.05f, -lakeL / 2);
    glTexCoord2f(1, 1); glVertex3f(lakeW / 2 + sideW, 0.05f, lakeL / 2); glTexCoord2f(0, 1); glVertex3f(lakeW / 2, 0.05f, lakeL / 2);
    glEnd(); glDisable(GL_TEXTURE_2D);

    drawFence(gardenW, gardenL);
    glPushMatrix(); glTranslatef(gardenW / 2, 0, 0); glRotatef(90, 0, 1, 0); drawMainGate(); glPopMatrix();

    for (int i = 0; i < 40; i++) {
        drawRock(-lakeW / 2 + (rand() % (int)lakeW), 0.05f, -lakeL / 2 + (rand() % 8) - 4, 2.5f);
        drawRock(-lakeW / 2 + (rand() % (int)lakeW), 0.05f, lakeL / 2 + (rand() % 8) - 4, 2.5f);
        drawRock(-lakeW / 2 + (rand() % 8) - 4, 0.05f, -lakeL / 2 + (rand() % (int)lakeL), 2.5f);
        drawRock(lakeW / 2 + (rand() % 8) - 4, 0.05f, -lakeL / 2 + (rand() % (int)lakeL), 2.5f);
    }
    for (int i = 0; i < 30; i++) {
        drawReed(-lakeW / 2 + 5, 0.0f, -lakeL / 2 + i * 10);
        drawReed(lakeW / 2 - 5, 0.0f, -lakeL / 2 + i * 10);
    }

    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, grassTex);
    glPushMatrix(); glTranslatef(0, -1.0f, 0); glScalef(30, 5, 30); glutSolidSphere(1, 20, 20); glPopMatrix();
    glDisable(GL_TEXTURE_2D); drawSimpleTree(0, 1.5f, 0);

    drawWoodenBridge(0, 0.1f, 50, 120.0f, 20.0f);
    drawGazebo(lakeW / 2 + 40, 0.1f, -lakeL / 2 + 40);
    drawJuiceStall(-gardenW / 2 + 40, 0.1f, gardenL / 2 - 40);

    float benchStart = -120;
    for (int i = 0; i < 5; i++) {
        float posZ = benchStart + i * 60;
        drawModernBench(lakeW / 2 + 20, 0.1f, posZ, -90);
        drawGardenLamp(lakeW / 2 + 35, 0.1f, posZ, isNight);
        drawModernBench(-lakeW / 2 - 20, 0.1f, posZ, 90);
        drawGardenLamp(-lakeW / 2 - 35, 0.1f, posZ, isNight);
    }
    for (int i = 0; i < 6; i++) {
        drawSimpleTree(-gardenW / 2 + 30, 0.1f, -gardenL / 2 + 30 + i * 80);
        drawSimpleTree(gardenW / 2 - 30, 0.1f, -gardenL / 2 + 30 + i * 80);
    }
    drawFlowerBed(gardenW / 2 - 40, 0.1f, -15);
    drawFlowerBed(gardenW / 2 - 40, 0.1f, 15);

    glPopMatrix();
}