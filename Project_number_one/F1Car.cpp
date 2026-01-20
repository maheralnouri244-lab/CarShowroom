#include "F1Car.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Define Static Color Constants
const float F1Car::COL_BLACK[] = { 0.15f, 0.15f, 0.15f };
const float F1Car::COL_GREY[] = { 0.7f, 0.7f, 0.7f };
const float F1Car::COL_DARK[] = { 0.1f, 0.1f, 0.1f };

// Constructor
F1Car::F1Car(float r, float g, float b) {
    bodyColor[0] = r;
    bodyColor[1] = g;
    bodyColor[2] = b;
}

// --- Helper Implementation ---

void F1Car::setColor(const float* color) {
    glColor3fv(color);
    float mat_amb[] = { color[0] * 0.5f, color[1] * 0.5f, color[2] * 0.5f, 1.0f };
    float mat_diff[] = { color[0], color[1], color[2], 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
}

void F1Car::drawCylinder(float radius, float height, int slices) {
    float x, y, angle;
    // Tube
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        angle = 2.0f * M_PI * i / slices;
        x = cos(angle) * radius; y = sin(angle) * radius;
        glNormal3f(cos(angle), sin(angle), 0.0f);
        glVertex3f(x, y, 0.0f); glVertex3f(x, y, height);
    }
    glEnd();
    // Top Cap
    glBegin(GL_TRIANGLE_FAN); glNormal3f(0, 0, 1); glVertex3f(0, 0, height);
    for (int i = 0; i <= slices; i++) {
        angle = 2.0f * M_PI * i / slices; glVertex3f(cos(angle) * radius, sin(angle) * radius, height);
    }
    glEnd();
    // Bottom Cap
    glBegin(GL_TRIANGLE_FAN); glNormal3f(0, 0, -1); glVertex3f(0, 0, 0);
    for (int i = slices; i >= 0; i--) {
        angle = 2.0f * M_PI * i / slices; glVertex3f(cos(angle) * radius, sin(angle) * radius, 0);
    }
    glEnd();
}

void F1Car::drawTaperedBox(float w_base, float w_tip, float h, float len) {
    float w1 = w_base / 2.0f, w2 = w_tip / 2.0f, hh = h / 2.0f;
    glBegin(GL_QUADS);
    // Top
    glNormal3f(0, 1, 0); glVertex3f(-w1, hh, 0); glVertex3f(-w2, hh, len); glVertex3f(w2, hh, len); glVertex3f(w1, hh, 0);
    // Bottom
    glNormal3f(0, -1, 0); glVertex3f(-w1, -hh, 0); glVertex3f(w1, -hh, 0); glVertex3f(w2, -hh, len); glVertex3f(-w2, -hh, len);
    // Left
    glNormal3f(-1, 0, 0); glVertex3f(-w1, -hh, 0); glVertex3f(-w2, -hh, len); glVertex3f(-w2, hh, len); glVertex3f(-w1, hh, 0);
    // Right
    glNormal3f(1, 0, 0); glVertex3f(w1, -hh, 0); glVertex3f(w1, hh, 0); glVertex3f(w2, hh, len); glVertex3f(w2, -hh, len);
    // Tip
    glNormal3f(0, 0, 1); glVertex3f(-w2, -hh, len); glVertex3f(w2, -hh, len); glVertex3f(w2, hh, len); glVertex3f(-w2, hh, len);
    // Base
    glNormal3f(0, 0, -1); glVertex3f(-w1, -hh, 0); glVertex3f(-w1, hh, 0); glVertex3f(w1, hh, 0); glVertex3f(w1, -hh, 0);
    glEnd();
}

// --- Component Drawers ---

void F1Car::drawWheel(bool isRear) {
    float radius = isRear ? 0.40f : 0.28f;
    float width = isRear ? 0.45f : 0.35f;

    glPushMatrix();
    glRotatef(90, 0, 1, 0);

    // Tire & Rim
    setColor(COL_BLACK); glutSolidTorus(radius * 0.35, radius * 0.65, 10, 20);
    setColor(COL_GREY); glutSolidTorus(radius * 0.08, radius * 0.55, 8, 20);

    // 5-Spoke Star Design
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glRotatef(72.0f * i, 0, 0, 1);
        glTranslatef(0.0f, radius * 0.25f, 0.0f);
        glScalef(width * 0.15f, radius * 0.55f, width * 0.08f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // Center Cap
    setColor(bodyColor);
    glPushMatrix(); glScalef(radius * 0.15f, radius * 0.15f, width * 0.2f); glutSolidCube(1.0f); glPopMatrix();
    glPopMatrix();
}

void F1Car::drawChassis() {
    setColor(bodyColor);
    // Main Body
    glPushMatrix(); glTranslatef(0, 0.2f, 0); glScalef(0.7f, 0.4f, 1.8f); glutSolidCube(1.0f); glPopMatrix();
    // Side Pods
    for (int s = -1; s <= 1; s += 2) {
        glPushMatrix(); glTranslatef(s * 0.6f, 0.2f, -0.2f); glScalef(0.4f, 0.4f, 1.2f); glutSolidCube(1.0f); glPopMatrix();
    }
    // Engine Cover
    glPushMatrix(); glTranslatef(0, 0.5f, -0.4f); glScalef(0.4f, 0.3f, 0.8f); glutSolidCube(1.0f); glPopMatrix();
    // Cockpit
    setColor(COL_DARK);
    glPushMatrix(); glTranslatef(0, 0.41f, 0.2f); glScalef(0.4f, 0.05f, 0.5f); glutSolidCube(1.0f); glPopMatrix();
}

void F1Car::drawNose() {
    setColor(bodyColor);
    glPushMatrix(); glTranslatef(0, 0.2f, 0.9f); drawTaperedBox(0.7f, 0.2f, 0.4f, 1.2f); glPopMatrix();
    setColor(COL_GREY);
    glPushMatrix(); glTranslatef(0, 0.41f, 0.9f); drawTaperedBox(0.4f, 0.1f, 0.02f, 1.0f); glPopMatrix();
}

void F1Car::drawWing(float w, float d, bool fins) {
    setColor(bodyColor);
    glPushMatrix(); glScalef(w, 0.05f, d); glutSolidCube(1.0f); glPopMatrix();
    if (fins) {
        for (int i = -1; i <= 1; i += 2) {
            glPushMatrix(); glTranslatef(i * w / 2.0f, 0.1f, 0); glScalef(0.05f, 0.3f, d); glutSolidCube(1.0f); glPopMatrix();
        }
    }
}

// --- Public Draw Method ---

void F1Car::draw() {
    float zPos[] = { 1.8f, -0.8f };
    float axleHeights[] = { 0.28f, 0.40f };
    bool isRear[] = { false, true };
    float wheelX = 0.95f;

    // Axles & Wheels
    for (int i = 0; i < 2; i++) {
        // Axle
        glPushMatrix();
        setColor(COL_DARK); glTranslatef(-wheelX, axleHeights[i], zPos[i]); glRotatef(90, 0, 1, 0);
        drawCylinder(0.05f, wheelX * 2.0f, 8);
        glPopMatrix();

        // Wheels
        for (int s = -1; s <= 1; s += 2) {
            glPushMatrix(); glTranslatef(s * wheelX, axleHeights[i], zPos[i]); drawWheel(isRear[i]); glPopMatrix();
        }
    }

    // Body Components
    drawChassis();
    drawNose();

    // Wings
    glPushMatrix(); glTranslatef(0, 0.15f, 2.2f); drawWing(1.8f, 0.4f, true); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.8f, -1.0f); drawWing(1.4f, 0.5f, false); glPopMatrix();

    // Struts
    setColor(bodyColor);
    for (int s = -1; s <= 1; s += 2) {
        glPushMatrix(); glTranslatef(s * 0.2f, 0.5f, -1.0f); glScalef(0.05f, 0.6f, 0.4f); glutSolidCube(1.0f); glPopMatrix();
    }
}
