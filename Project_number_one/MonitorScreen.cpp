#include "MonitorScreen.h"

MonitorScreen::MonitorScreen(float w, float h, float d) : width(w), height(h), depth(d) {
    standHeight = h * 0.15f;
    standRadius = 0.6f;
    baseRadius = w * 0.2f;
    baseHeight = 0.5f;
    bodyColor[0] = 0.1f; bodyColor[1] = 0.1f; bodyColor[2] = 0.1f; bodyColor[3] = 1.0f;
}

void MonitorScreen::draw(GLuint textures[4]) {
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    drawBase();

    glTranslatef(0.0f, baseHeight, 0.0f);
    drawStand();

    glTranslatef(0.0f, standHeight + (height / 2.0f), 0.0f);
    drawMonitorHead(textures);

    glPopAttrib();
    glPopMatrix();
}

void MonitorScreen::drawBase() {
    GLfloat grey[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(baseRadius, baseHeight, 30, 5);
    glPopMatrix();
}

void MonitorScreen::drawStand() {
    GLfloat grey[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, standRadius, standRadius, standHeight, 20, 5);
    gluDeleteQuadric(quad);
    glPopMatrix();
}

void MonitorScreen::drawMonitorHead(GLuint textures[4]) {
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    float zFront = (depth / 2.0f) + 0.05f;

    GLfloat grey[] = { 0.12f, 0.12f, 0.12f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);

    glPushMatrix();
    glScalef(width, height, depth);
    glutSolidCube(1.0);
    glPopMatrix();

    drawQuadrant(-halfW, 0.0f, 0.0f, halfH, textures[0], zFront);
    drawQuadrant(0.0f, 0.0f, halfW, halfH, textures[1], zFront);
    drawQuadrant(-halfW, -halfH, 0.0f, 0.0f, textures[2], zFront);
    drawQuadrant(0.0f, -halfH, halfW, 0.0f, textures[3], zFront);
}

void MonitorScreen::drawQuadrant(float x1, float y1, float x2, float y2, GLuint texID, float zPos) {
    if (texID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID);
        GLfloat emission[] = { 0.7f, 0.7f, 0.7f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(x1, y1, zPos);
        glTexCoord2f(1, 0); glVertex3f(x2, y1, zPos);
        glTexCoord2f(1, 1); glVertex3f(x2, y2, zPos);
        glTexCoord2f(0, 1); glVertex3f(x1, y2, zPos);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    }
    else {
        glDisable(GL_TEXTURE_2D);
        GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glVertex3f(x1, y1, zPos);
        glVertex3f(x2, y1, zPos);
        glVertex3f(x2, y2, zPos);
        glVertex3f(x1, y2, zPos);
        glEnd();
    }

    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x1, y1, zPos + 0.01f);
    glVertex3f(x2, y1, zPos + 0.01f);
    glVertex3f(x2, y2, zPos + 0.01f);
    glVertex3f(x1, y2, zPos + 0.01f);
    glEnd();
    glEnable(GL_LIGHTING);
}