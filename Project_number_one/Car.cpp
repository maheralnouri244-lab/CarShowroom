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
    glPushMatrix(); 
    glScalef(w, h, l); 
    glutSolidCube(1.0f); 
    glPopMatrix();
}

void Car::drawCabin(float w, float h, float l, float topRatio, float r, float g, float b) {
    float hw = w / 2; 
    float hl = l / 2; 
    float hh = h / 2; 
    float topL = hl * topRatio;

    glBegin(GL_QUADS);
    glColor3f(r, g, b); 
    glNormal3f(0, 1, 0); 
    glVertex3f(-hw, hh, -topL); 
    glVertex3f(hw, hh, -topL);
    glVertex3f(hw, hh, topL); 
    glVertex3f(-hw, hh, topL);

    glNormal3f(0, -1, 0); 
    glVertex3f(-hw, -hh, -hl); 
    glVertex3f(hw, -hh, -hl); 
    glVertex3f(hw, -hh, hl); 
    glVertex3f(-hw, -hh, hl);

    glColor3f(0.2f, 0.3f, 0.4f); 
    glNormal3f(0, 0.7f, 0.7f); 
    glVertex3f(-hw, -hh, hl); 
    glVertex3f(hw, -hh, hl); 
    glVertex3f(hw, hh, topL); 
    glVertex3f(-hw, hh, topL);

    glNormal3f(0, 0.7f, -0.7f); 
    glVertex3f(-hw, hh, -topL); 
    glVertex3f(hw, hh, -topL); 
    glVertex3f(hw, -hh, -hl); 
    glVertex3f(-hw, -hh, -hl);

    glColor3f(r, g, b); 
    glNormal3f(1, 0, 0); 
    glVertex3f(hw, -hh, -hl);
    glVertex3f(hw, hh, -topL);
    glVertex3f(hw, hh, topL);
    glVertex3f(hw, -hh, hl);

    glNormal3f(-1, 0, 0); 
    glVertex3f(-hw, -hh, hl);
    glVertex3f(-hw, hh, topL); 
    glVertex3f(-hw, hh, -topL); 
    glVertex3f(-hw, -hh, -hl);

    glEnd();
}