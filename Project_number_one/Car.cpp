#include "Car.h"

Car::Car(float _x, float _y, float _z, float _rot, float _r, float _g, float _b, float _scale)
    : x(_x), y(_y), z(_z), rotation(_rot), r(_r), g(_g), b(_b), scale(_scale) {
}

void Car::drawWheel(float dx, float dy, float dz, float radius, float width) {
    glPushMatrix();
    glTranslatef(dx, dy, dz);
    glRotatef(rotation, 0, 1, 0);
    glColor3f(0.15f, 0.15f, 0.15f);
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, radius, radius, width, 16, 1);
    gluDisk(q, 0, radius, 16, 1);
    glTranslatef(0, 0, width);
    gluDisk(q, 0, radius, 16, 1);
    glTranslatef(0, 0, 0.05f);
    glColor3f(0.7f, 0.7f, 0.7f);
    gluDisk(q, 0, radius * 0.6f, 16, 1);
    gluDeleteQuadric(q);
    glPopMatrix();
    glPopMatrix();
}

void Car::drawChassis(float w, float h, float l, float r, float g, float b) {
    glColor3f(r, g, b);
    glPushMatrix(); glScalef(w, h, l); glutSolidCube(1.0f); glPopMatrix();
}

void Car::drawCabin(float w, float h, float l, float topRatio, float r, float g, float b) {
    float hw = w / 2; float hl = l / 2; float hh = h / 2; float topL = hl * topRatio;
    glBegin(GL_QUADS);
    glColor3f(r, g, b); glNormal3f(0, 1, 0); glVertex3f(-hw, hh, -topL); glVertex3f(hw, hh, -topL); glVertex3f(hw, hh, topL); glVertex3f(-hw, hh, topL);
    glNormal3f(0, -1, 0); glVertex3f(-hw, -hh, -hl); glVertex3f(hw, -hh, -hl); glVertex3f(hw, -hh, hl); glVertex3f(-hw, -hh, hl);
    glColor3f(0.2f, 0.3f, 0.4f); glNormal3f(0, 0.7f, 0.7f); glVertex3f(-hw, -hh, hl); glVertex3f(hw, -hh, hl); glVertex3f(hw, hh, topL); glVertex3f(-hw, hh, topL);
    glNormal3f(0, 0.7f, -0.7f); glVertex3f(-hw, hh, -topL); glVertex3f(hw, hh, -topL); glVertex3f(hw, -hh, -hl); glVertex3f(-hw, -hh, -hl);
    glColor3f(r, g, b); glNormal3f(1, 0, 0); glVertex3f(hw, -hh, -hl); glVertex3f(hw, hh, -topL); glVertex3f(hw, hh, topL); glVertex3f(hw, -hh, hl);
    glNormal3f(-1, 0, 0); glVertex3f(-hw, -hh, hl); glVertex3f(-hw, hh, topL); glVertex3f(-hw, hh, -topL); glVertex3f(-hw, -hh, -hl);
    glEnd();
}

void Sedan::draw() {
    glPushMatrix();
    glTranslatef(x, y + 2.5f, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale * 2.0f, scale * 2.0f, scale * 2.0f);
    glDisable(GL_TEXTURE_2D);

    drawChassis(2.2f, 0.7f, 5.0f, r, g, b);
    glPushMatrix(); glTranslatef(0, 0.7f, 0); drawCabin(2.0f, 0.6f, 3.0f, 0.6f, r, g, b); glPopMatrix();

    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix(); glTranslatef(0, -0.1f, 2.55f); glScalef(2.0f, 0.3f, 0.2f); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(0, -0.1f, -2.55f); glScalef(2.0f, 0.3f, 0.2f); glutSolidCube(1); glPopMatrix();

    glRotatef(-rotation, 0, 1, 0);
    float wR = 0.35f; float wW = 0.3f;
    drawWheel(1.0f, -0.35f, 1.5f, wR, wW); drawWheel(-1.3f, -0.35f, 1.5f, wR, wW);
    drawWheel(1.0f, -0.35f, -1.5f, wR, wW); drawWheel(-1.3f, -0.35f, -1.5f, wR, wW);
    glPopMatrix();
}

void SUV::draw() {
    glPushMatrix();
    glTranslatef(x, y + 3.2f, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale * 2.0f, scale * 2.0f, scale * 2.0f);
    glDisable(GL_TEXTURE_2D);

    drawChassis(2.4f, 0.9f, 5.5f, r, g, b);
    glPushMatrix(); glTranslatef(0, 0.9f, -0.2f); drawCabin(2.3f, 0.8f, 3.5f, 0.8f, r, g, b); glPopMatrix();

    glRotatef(-rotation, 0, 1, 0);
    float wR = 0.45f; float wW = 0.4f;
    drawWheel(1.1f, -0.45f, 1.8f, wR, wW); drawWheel(-1.5f, -0.45f, 1.8f, wR, wW);
    drawWheel(1.1f, -0.45f, -1.8f, wR, wW); drawWheel(-1.5f, -0.45f, -1.8f, wR, wW);
    glPopMatrix();
}

void SportsCar::draw() {
    glPushMatrix();
    glTranslatef(x, y + 2.0f, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale * 2.0f, scale * 1.6f, scale * 2.0f);
    glDisable(GL_TEXTURE_2D);

    drawChassis(2.4f, 0.6f, 5.2f, r, g, b);
    glPushMatrix(); glTranslatef(0, 0.6f, -0.2f); drawCabin(2.0f, 0.5f, 2.5f, 0.5f, r, g, b); glPopMatrix();

    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix(); glTranslatef(0, 0.8f, -2.4f); glScalef(2.2f, 0.1f, 0.5f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.8f, 0.6f, -2.4f); glScalef(0.1f, 0.4f, 0.2f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.8f, 0.6f, -2.4f); glScalef(0.1f, 0.4f, 0.2f); glutSolidCube(1.0f); glPopMatrix();

    glRotatef(-rotation, 0, 1, 0);
    float wR = 0.38f; float wW = 0.45f;
    drawWheel(1.2f, -0.3f, 1.7f, wR, wW); drawWheel(-1.6f, -0.3f, 1.7f, wR, wW);
    drawWheel(1.2f, -0.3f, -1.7f, wR, wW); drawWheel(-1.6f, -0.3f, -1.7f, wR, wW);
    glPopMatrix();
}

void Taxi::draw() {
    glPushMatrix();
    glTranslatef(x, y + 2.5f, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale * 2.0f, scale * 2.0f, scale * 2.0f);
    glDisable(GL_TEXTURE_2D);

    drawChassis(2.2f, 0.7f, 5.0f, 1.0f, 0.8f, 0.0f);
    glPushMatrix(); glTranslatef(0, 0.7f, 0); drawCabin(2.0f, 0.6f, 3.0f, 0.6f, 1.0f, 0.8f, 0.0f); glPopMatrix();

    // ????? ???????
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix(); glTranslatef(0, 1.4f, 0); glScalef(0.8f, 0.2f, 0.3f); glutSolidCube(1.0f); glPopMatrix();

    glRotatef(-rotation, 0, 1, 0);
    float wR = 0.35f; float wW = 0.3f;
    drawWheel(1.0f, -0.35f, 1.5f, wR, wW); drawWheel(-1.3f, -0.35f, 1.5f, wR, wW);
    drawWheel(1.0f, -0.35f, -1.5f, wR, wW); drawWheel(-1.3f, -0.35f, -1.5f, wR, wW);
    glPopMatrix();
}

void Ambulance::draw() {
    glPushMatrix();
    glTranslatef(x, y + 3.0f, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale * 2.0f, scale * 2.0f, scale * 2.0f);
    glDisable(GL_TEXTURE_2D);

    drawChassis(2.4f, 1.0f, 2.0f, 0.95f, 0.95f, 0.95f);
    glPushMatrix(); glTranslatef(0, 0.2f, -1.5f); glColor3f(0.95f, 0.95f, 0.95f); glScalef(2.4f, 1.8f, 3.5f); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 1.0f, 1.0f); drawCabin(2.2f, 0.6f, 1.5f, 0.7f, 0.95f, 0.95f, 0.95f); glPopMatrix();

    // ???? ????
    glColor3f(0.9f, 0.0f, 0.0f);
    glPushMatrix(); glTranslatef(1.21f, 0.5f, -1.5f); glScalef(0.1f, 1.0f, 0.3f); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(1.21f, 0.5f, -1.5f); glScalef(0.1f, 0.3f, 1.0f); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(-1.21f, 0.5f, -1.5f); glScalef(0.1f, 1.0f, 0.3f); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(-1.21f, 0.5f, -1.5f); glScalef(0.1f, 0.3f, 1.0f); glutSolidCube(1); glPopMatrix();

    glRotatef(-rotation, 0, 1, 0);
    float wR = 0.4f; float wW = 0.4f;
    drawWheel(1.1f, -0.5f, 1.5f, wR, wW); drawWheel(-1.5f, -0.5f, 1.5f, wR, wW);
    drawWheel(1.1f, -0.5f, -2.0f, wR, wW); drawWheel(-1.5f, -0.5f, -2.0f, wR, wW);
    glPopMatrix();
}
