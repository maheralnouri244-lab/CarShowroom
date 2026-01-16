#include "SteveModel.h"

float SteveModel::SkinColor[3] = { 0.70f, 0.47f, 0.35f };
float SteveModel::HairColor[3] = { 0.25f, 0.15f, 0.10f };
float SteveModel::ShirtColor[3] = { 0.0f, 0.65f, 0.65f };
float SteveModel::PantsColor[3] = { 0.25f, 0.25f, 0.60f };
float SteveModel::ShoeColor[3] = { 0.45f, 0.45f, 0.45f };

SteveModel::SteveModel(float baseUnit) : unit(baseUnit) {}

void SteveModel::drawBox(float width, float height, float depth) {
    float w = width / 2.0f;
    float h = height / 2.0f;
    float d = depth / 2.0f;

    glBegin(GL_QUADS);
    // «·√„«„
    glNormal3f(0, 0, 1);
    glVertex3f(-w, -h, d); glVertex3f(w, -h, d);
    glVertex3f(w, h, d); glVertex3f(-w, h, d);
    // «·Œ·›
    glNormal3f(0, 0, -1);
    glVertex3f(-w, -h, -d); glVertex3f(-w, h, -d);
    glVertex3f(w, h, -d); glVertex3f(w, -h, -d);
    // «·√⁄·Ï
    glNormal3f(0, 1, 0);
    glVertex3f(-w, h, -d); glVertex3f(-w, h, d);
    glVertex3f(w, h, d); glVertex3f(w, h, -d);
    // «·√”›·
    glNormal3f(0, -1, 0);
    glVertex3f(-w, -h, -d); glVertex3f(w, -h, -d);
    glVertex3f(w, h, d); glVertex3f(-w, -h, d);
    // «·Ì„Ì‰
    glNormal3f(1, 0, 0);
    glVertex3f(w, -h, -d); glVertex3f(w, h, -d);
    glVertex3f(w, h, d); glVertex3f(w, -h, d);
    // «·Ì”«—
    glNormal3f(-1, 0, 0);
    glVertex3f(-w, -h, -d); glVertex3f(-w, -h, d);
    glVertex3f(-w, h, d); glVertex3f(-w, h, -d);
    glEnd();
}

void SteveModel::draw() {
    glPushMatrix();
    drawLegs();
    drawTorso();
    drawArms();
    drawHead();
    glPopMatrix();
}

void SteveModel::drawHead() {
    glPushMatrix();
    glTranslatef(0.0f, (12 + 12 + 4) * unit, 0.0f);
    glColor3fv(SkinColor);
    drawBox(8 * unit, 8 * unit, 8 * unit);

    glColor3fv(HairColor);
    glPushMatrix();
    glTranslatef(0.0f, 0.6 * unit, 0.1f);
    drawBox(8.02 * unit, 7.5 * unit, 8.02 * unit);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 3 * unit, 0.0f);
    drawBox(8.02 * unit, 3 * unit, 8.02 * unit);
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-1.9f * unit, 1.0f * unit, -4.05f * unit);
    drawBox(0.9f * unit, 1.2f * unit, 0.2f * unit);
    glPopMatrix();

    glColor3f(0.15f, 0.25f, 0.85f);
    glPushMatrix();
    glTranslatef(-1.1f * unit, 1.0f * unit, -4.05f * unit);
    drawBox(0.9f * unit, 1.2f * unit, 0.2f * unit);
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(1.9f * unit, 1.0f * unit, -4.05f * unit);
    drawBox(0.9f * unit, 1.2f * unit, 0.2f * unit);
    glPopMatrix();

    glColor3f(0.15f, 0.25f, 0.85f);
    glPushMatrix();
    glTranslatef(1.1f * unit, 1.0f * unit, -4.05f * unit);
    drawBox(0.9f * unit, 1.2f * unit, 0.2f * unit);
    glPopMatrix();


    // «·›„
    glColor3f(0.35f, 0.20f, 0.15f);
    glPushMatrix();
    glTranslatef(0.0f, -1.8f * unit, -4.05f * unit);
    drawBox(2.5f * unit, 0.6f * unit, 0.2f * unit);
    glPopMatrix();

    glPopMatrix();
}

void SteveModel::drawTorso() {
    glPushMatrix();
    glTranslatef(0.0f, (12 + 6) * unit, 0.0f);
    glColor3fv(ShirtColor);
    drawBox(8 * unit, 12 * unit, 4 * unit);
    glPopMatrix();
}

void SteveModel::drawArms() {
    glPushMatrix();
    glTranslatef(-6 * unit, 18 * unit, 0.0f);
    //glRotatef(10.0f, 0, 0, 1);
    glColor3fv(SkinColor);
    drawBox(4 * unit, 12 * unit, 4 * unit);
    glColor3fv(ShirtColor);
    glTranslatef(0.0f, 4.5 * unit, 0.0f);
    drawBox(4.05 * unit, 4 * unit, 4.05 * unit);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(6 * unit, 18 * unit, 0.0f);
    //glRotatef(-10.0f, 0, 0, 1);
    glColor3fv(SkinColor);
    drawBox(4 * unit, 12 * unit, 4 * unit);
    glColor3fv(ShirtColor);
    glTranslatef(0.0f, 4.5 * unit, 0.0f);
    drawBox(4.05 * unit, 4 * unit, 4.05 * unit);
    glPopMatrix();
}

void SteveModel::drawLegs() {
    glPushMatrix();
    glTranslatef(-2 * unit, 6 * unit, 0.0f);
    glColor3fv(PantsColor);
    drawBox(4 * unit, 12 * unit, 4 * unit);
    // «·Õ–«¡
    glTranslatef(0.0f, -5 * unit, 0.0f);
    glColor3fv(ShoeColor);
    drawBox(4.02 * unit, 2.1 * unit, 4.02 * unit);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2 * unit, 6 * unit, 0.0f);
    glColor3fv(PantsColor);
    drawBox(4 * unit, 12 * unit, 4 * unit);
    glTranslatef(0.0f, -5 * unit, 0.0f);
    glColor3fv(ShoeColor);
    drawBox(4.02 * unit, 2.1 * unit, 4.02 * unit);
    glPopMatrix();
}

void SteveModel::setSkinColor(float r, float g, float b)
{
    SkinColor[0] = r;
    SkinColor[1] = g;
    SkinColor[2] = b;
}

void SteveModel::setHairColor(float r, float g, float b)
{
    HairColor[0] = r;
    HairColor[1] = g;
    HairColor[2] = b;
}

void SteveModel::setShirtColor(float r, float g, float b)
{
    ShirtColor[0] = r;
    ShirtColor[1] = g;
    ShirtColor[2] = b;
}

void SteveModel::setPantsColor(float r, float g, float b)
{
    PantsColor[0] = r;
    PantsColor[1] = g;
    PantsColor[2] = b;
}

void SteveModel::setShoeColor(float r, float g, float b)
{
    ShoeColor[0] = r;
    ShoeColor[1] = g;
    ShoeColor[2] = b;
}
