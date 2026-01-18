#include "BeetleCar.h"
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// الألوان
const float CAR_PALETTE[4][4] = {
    { 0.1f, 0.2f, 0.7f, 1.0f }, // Blue
    { 0.7f, 0.1f, 0.1f, 1.0f }, // Red
    { 0.2f, 0.6f, 0.2f, 1.0f }, // Green
    { 0.8f, 0.7f, 0.1f, 1.0f }  // Yellow
};

const float COL_RED[] = { 0.9f, 0.05f, 0.05f, 1.0f };
const float COL_BLACK[] = { 0.1f, 0.1f, 0.1f, 1.0f };
const float COL_TIRE[] = { 0.15f, 0.15f, 0.15f, 1.0f };
const float COL_CHROME[] = { 0.95f, 0.95f, 1.0f, 1.0f };
const float COL_WHITE[] = { 1.0f, 1.0f, 1.0f, 1.0f };
// زجاج شفاف جداً (Alpha = 0.2)
const float COL_GLASS[] = { 0.85f, 0.9f, 1.0f, 0.2f };
const float COL_SKIN[] = { 0.85f, 0.65f, 0.55f, 1.0f };

BeetleCar::BeetleCar() {
    currentBodyColor = CAR_PALETTE[0];
}

// دوال فارغة للتوافق (المنطق انتقل لـ AbrarCode)
void BeetleCar::toggleDoors() {}
void BeetleCar::updateAnimation() {}

// --- Helpers ---
void BeetleCar::setMaterial(const float* color, float shine, float alphaOverride) {
    float alpha = (alphaOverride >= 0.0f) ? alphaOverride : color[3];
    glColor4f(color[0], color[1], color[2], alpha);
    float mat_ambient[] = { color[0] * 0.4f, color[1] * 0.4f, color[2] * 0.4f, alpha };
    float mat_diffuse[] = { color[0], color[1], color[2], alpha };
    float mat_specular[] = { shine, shine, shine, alpha };
    float mat_shininess[] = { shine * 128.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void BeetleCar::drawDisk(float radius, int slices) {
    glBegin(GL_TRIANGLE_FAN); glNormal3f(0, 0, 1); glVertex3f(0, 0, 0);
    for (int i = 0; i <= slices; i++) { float a = 2.0f * M_PI * i / slices; glVertex3f(cos(a) * radius, sin(a) * radius, 0); } glEnd();
}

void BeetleCar::drawCylinder(float radius, float height, int slices) {
    float x, y, a; glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) { a = 2.0f * M_PI * i / slices; x = cos(a) * radius; y = sin(a) * radius; glNormal3f(x / radius, y / radius, 0); glVertex3f(x, y, 0); glVertex3f(x, y, height); } glEnd();
    glPushMatrix(); glTranslatef(0, 0, height); drawDisk(radius, slices); glPopMatrix();
    glPushMatrix(); glRotatef(180, 1, 0, 0); drawDisk(radius, slices); glPopMatrix();
}

// --- Parts ---
void BeetleCar::drawWhitewallWheel() {
    float r = 0.38f, w = 0.28f, rim = 0.23f;
    glPushMatrix(); glRotatef(90, 0, 1, 0);
    setMaterial(COL_TIRE, 0.1f);
    glPushMatrix(); glTranslatef(0, 0, -w / 2); drawCylinder(r - 0.02f, w, 24); glPopMatrix();
    for (int i = 0;i < 40;i++) { glPushMatrix(); glRotatef(i * 9, 0, 0, 1); glTranslatef(r, 0, 0); glScalef(0.04f, 0.02f, w * 0.95f); glutSolidCube(1.0f); glPopMatrix(); }
    glPushMatrix(); glTranslatef(0, 0, -w / 2 + 0.02f); glutSolidTorus(0.04, r - 0.04, 8, 30); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0, w / 2 - 0.02f); glutSolidTorus(0.04, r - 0.04, 8, 30); glPopMatrix();
    setMaterial(COL_WHITE, 0.05f); glPushMatrix(); glTranslatef(0, 0, w / 2 - 0.015f); glutSolidTorus(0.025, 0.30, 8, 40); glPopMatrix();
    setMaterial(COL_CHROME, 0.9f); glPushMatrix(); glTranslatef(0, 0, w / 2 - 0.06f); glutSolidTorus(0.03, rim, 8, 30); glPopMatrix();
    for (int i = 0;i < 10;i++) { glPushMatrix(); glTranslatef(0, 0, w / 2 - 0.08f); glRotatef(i * 36, 0, 0, 1); glTranslatef(0, rim * 0.5f, 0); glScalef(0.04f, 0.5f, 0.02f); glutSolidCube(1.0f); glPopMatrix(); }
    glPushMatrix(); glTranslatef(0, 0, w / 2 - 0.06f); drawDisk(0.08f, 16); glPopMatrix(); glPopMatrix();
}

void BeetleCar::drawSeat() {
    setMaterial(COL_BLACK, 0.1f); glPushMatrix(); glScalef(0.45f, 0.15f, 0.5f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.35f, -0.2f); glRotatef(-10, 1, 0, 0); glScalef(0.45f, 0.6f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.7f, -0.25f); glScalef(0.3f, 0.15f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
}

void BeetleCar::drawGenericDoor(bool isRight, bool hasMirror) {
    float s = isRight ? 1.0f : -1.0f;
    setMaterial(currentBodyColor, 0.8f); glPushMatrix(); glTranslatef(0, 0.25f, -0.47f); glScalef(0.1f, 0.5f, 0.95f); glutSolidCube(1.0f); glPopMatrix();
    setMaterial(COL_CHROME, 0.9f); glPushMatrix(); glTranslatef(s * 0.01f, 0.1f, -0.47f); glScalef(0.12f, 0.05f, 0.95f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(s * 0.02f, 0.35f, -0.8f); glScalef(0.15f, 0.03f, 0.15f); glutSolidCube(1.0f); glPopMatrix();
    if (hasMirror) {
        setMaterial(COL_BLACK, 0.2f); glPushMatrix(); glTranslatef(s * 0.05f, 0.55f, -0.1f); glRotatef(s * -30, 0, 1, 0); glScalef(0.2f, 0.04f, 0.04f); glutSolidCube(1.0f);
        setMaterial(currentBodyColor, 0.9f); glTranslatef(s * 0.8f, 0, 0); glScalef(1.0f, 3.0f, 2.0f); glutSolidSphere(0.1f, 10, 10);
        setMaterial(COL_CHROME, 1.0f); glTranslatef(0, 0, -0.5f); glScalef(0.8f, 0.8f, 0.1f); glutSolidSphere(1.0f, 8, 8); glPopMatrix();
    }
}

void BeetleCar::drawExtendedChassis() {
    setMaterial(COL_BLACK, 0.1f); glPushMatrix(); glTranslatef(0, 0.2f, 0); glScalef(1.5f, 0.1f, 3.0f); glutSolidCube(1.0f); glPopMatrix();
    setMaterial(currentBodyColor, 0.8f); glPushMatrix(); glTranslatef(0, 0.45f, 1.6f); glScalef(1.6f, 0.55f, 0.8f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.45f, 0.15f); glScalef(1.55f, 0.55f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.45f, 1.8f); glPushMatrix(); glRotatef(15, 1, 0, 0); glScalef(1.5f, 0.4f, 1.1f); glutSolidCube(1.0f); glPopMatrix();
    glTranslatef(0, -0.2f, 0.5f); glScalef(1.5f, 0.35f, 0.4f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.45f, -1.6f); glScalef(1.6f, 0.55f, 0.8f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.3f, -1.9f); glPushMatrix(); glScalef(1.55f, 0.8f, 1.3f); glutSolidSphere(0.7f, 24, 24); glPopMatrix();
    glTranslatef(0, 0, 0.4f); glScalef(1.5f, 0.3f, 0.4f); glutSolidCube(1.0f); glPopMatrix();
    float zF = 1.6f, zR = -1.6f;
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix(); glTranslatef(i * 0.65f, 0.5f, zF); glRotatef(90, 0, 1, 0); glScalef(1.2f, 0.6f, 0.5f); glutSolidSphere(0.6f, 20, 20); glPopMatrix();
        glPushMatrix(); glTranslatef(i * 0.65f, 0.5f, zR); glRotatef(90, 0, 1, 0); glScalef(1.2f, 0.65f, 0.55f); glutSolidSphere(0.6f, 20, 20); glPopMatrix();
    }
}

void BeetleCar::drawFrontEnd() {
    setMaterial(currentBodyColor, 0.8f);
    glPushMatrix(); glTranslatef(0.0f, 0.45f, 1.8f); glPushMatrix(); glRotatef(15, 1, 0, 0); glScalef(1.5f, 0.45f, 1.2f); glutSolidCube(1.0f); glPopMatrix();
    glTranslatef(0.0f, -0.25f, 0.6f); glPushMatrix(); glScalef(1.4f, 0.35f, 0.5f); glutSolidSphere(1.0f, 20, 20); glPopMatrix(); glPopMatrix();
    setMaterial(COL_CHROME, 1.0f); glPushMatrix(); glTranslatef(0.0f, 0.52f, 2.3f); glRotatef(-30, 1, 0, 0); drawDisk(0.08f, 16);
    setMaterial(COL_BLACK); glTranslatef(0, 0, 0.01f); drawDisk(0.06f, 12); glPopMatrix();
    setMaterial(COL_BLACK, 0.2f); glPushMatrix(); glTranslatef(0.0f, 0.15f, 2.38f); glScalef(1.0f, 0.1f, 0.05f); glutSolidCube(1.0f); glPopMatrix();
}

void BeetleCar::drawOpenCabin(float steeringAngle) {
    float y = 0.7f, z = 1.0f;
    setMaterial(currentBodyColor, 0.8f); 
    glPushMatrix(); 
    glTranslatef(0, y + 0.35f, z); 
    glRotatef(-30, 1, 0, 0);
    for (int i = -1;i <= 1;i += 2) { 
        glPushMatrix(); 
        glTranslatef(i * 0.75f, 0, 0); 
        glScalef(0.1f, 0.8f, 0.1f); 
        glutSolidCube(1.0f); 
        glPopMatrix(); 
    }

    glPushMatrix(); 
    glTranslatef(0, 0.4f, 0);
    glScalef(1.6f, 0.1f, 0.1f); 
    glutSolidCube(1.0f); 
    glPopMatrix(); 
    glPopMatrix();

    for (int i = -1;i <= 1;i += 2) { 
        glPushMatrix();
        glTranslatef(i * 0.4f, 0.45f, 0.5f); 
        drawSeat(); 
        glPopMatrix(); 
    }
    glPushMatrix(); 
    glTranslatef(0, 0.5f, -1.0f); 
    glScalef(0.9f, 0.9f, 0.9f); 
    drawSeat(); 
    glPopMatrix();

    setMaterial(currentBodyColor, 0.9f); 
    glPushMatrix(); 
    glTranslatef(0, y + 0.35f, -1.55f); 
    glRotatef(20, 1, 0, 0);
    glPushMatrix(); 
    glScalef(1.55f, 0.6f, 0.1f); 
    glutSolidCube(1.0f); 
    glPopMatrix(); 
    glPopMatrix();

    setMaterial(COL_BLACK, 0.2f); 
    glPushMatrix();
    glTranslatef(0, 0.65f, 1.2f); 
    glScalef(1.4f, 0.3f, 0.4f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    setMaterial(COL_BLACK, 0.1f); 
    glPushMatrix(); 
    glTranslatef(0.4f, 0.85f, 0.9f); 
    glRotatef(-25, 1, 0, 0); 
    ////
    glRotatef(steeringAngle, 0, 0, 1);

    glutSolidTorus(0.02, 0.16, 8, 20); // الحلقة
    glPushMatrix(); glScalef(0.03f, 0.3f, 0.01f); glutSolidCube(1.0f); glPopMatrix(); // ذراع عمودي
    glPushMatrix(); glRotatef(90, 0, 0, 1); glScalef(0.03f, 0.3f, 0.01f); glutSolidCube(1.0f); glPopMatrix(); // ذراع أفقي
    glPopMatrix();
}

void BeetleCar::drawDetails() {
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix(); 
        glTranslatef(i * 0.55f, 0.55f, 2.2f); 
        glRotatef(-20, 1, 0, 0); 
        setMaterial(COL_CHROME, 0.9f); 
        glutSolidTorus(0.03, 0.13, 12, 20);

        setMaterial(COL_WHITE, 1.0f); 
        glTranslatef(0, 0, 0.02f); 
        drawDisk(0.13f, 16); 
        glPopMatrix();
    }
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix(); 
        glTranslatef(i * 0.6f, 0.65f, -2.2f); 
        setMaterial(COL_RED, 0.6f); 
        glScalef(1.0f, 0.8f, 0.2f); 
        glutSolidSphere(0.12f, 12, 12); 
        glPopMatrix();
    }
}

void BeetleCar::drawGlass() {
    float y = 0.7f, z = 1.0f;
    // Transparency Settings (Draw without depth mask to see through)
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    setMaterial(COL_GLASS, 0.9f, 0.2f); // High shine, low alpha
    glPushMatrix(); glTranslatef(0, y + 0.35f, z); glRotatef(-30, 1, 0, 0); glScalef(1.5f, 0.8f, 0.05f); glutSolidCube(1.0f); glPopMatrix();
    glDepthMask(GL_TRUE); glDisable(GL_BLEND);
}

void BeetleCar::drawHands(float steeringAngle) {
    setMaterial(COL_SKIN, 0.1f);

    glPushMatrix();
    glTranslatef(0.4f, 0.85f, 0.9f); // مركز المقود
    glRotatef(-25, 1, 0, 0); // نفس ميلان المقود
    glRotatef(steeringAngle, 0, 0, 1); // نفس دوران المقود

    // اليدين مثبتتان على المقود (مسافة 0.16 من المركز)
    // Left Hand (at 9 o'clock -> -0.15 X)
    glPushMatrix(); glTranslatef(-0.15f, 0.0f, 0.0f); glutSolidSphere(0.06f, 10, 10); glPopMatrix();
    // Right Hand (at 3 o'clock -> 0.15 X)
    glPushMatrix(); glTranslatef(0.15f, 0.0f, 0.0f); glutSolidSphere(0.06f, 10, 10); glPopMatrix();

    glPopMatrix();
}


void BeetleCar::draw(float x, float y, float z, float scale, float rotateY, int colorIndex, bool isDriving, float doorAngles[], float steeringAngle) {
    currentBodyColor = CAR_PALETTE[colorIndex % 4];

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotateY, 0, 1, 0);
    glScalef(scale, scale, scale);

    // 1. Opaque Parts First
    drawExtendedChassis();
    drawFrontEnd();
    drawOpenCabin(steeringAngle);
    drawDetails();

    float W = 0.9f, F = 1.6f, R = -1.6f; float pos[4][3] = { {W,0,F},{-W,0,F},{W,0,R},{-W,0,R} };
    for (int i = 0;i < 4;i++) { glPushMatrix(); glTranslatef(pos[i][0], 0.38f, pos[i][2]); drawWhitewallWheel(); glPopMatrix(); }

    float sX = 0.78f, hF = 1.1f, hR = 0.15f;
    glPushMatrix(); glTranslatef(-sX, 0.3f, hF); glRotatef(doorAngles[0], 0, 1, 0); drawGenericDoor(false, true); glPopMatrix();
    glPushMatrix(); glTranslatef(sX, 0.3f, hF);  glRotatef(-doorAngles[1], 0, 1, 0); drawGenericDoor(true, true); glPopMatrix();
    glPushMatrix(); glTranslatef(-sX, 0.3f, hR); glRotatef(doorAngles[2], 0, 1, 0); drawGenericDoor(false, false); glPopMatrix();
    glPushMatrix(); glTranslatef(sX, 0.3f, hR);  glRotatef(-doorAngles[3], 0, 1, 0); drawGenericDoor(true, false); glPopMatrix();

    // 2. Transparent Parts Last
    drawGlass();

    // 3. Hands (if driving)
    if (isDriving) {
        drawHands(steeringAngle);
    }

    glPopMatrix();
}
