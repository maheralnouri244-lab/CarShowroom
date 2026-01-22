#include "StreetFurniture.h"

void StreetFurniture::drawBench(float x, float y, float z, float rotation) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glRotatef(rotation, 0, 1, 0); 
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.2f, 0.2f, 0.2f); 
    glPushMatrix(); 
    glTranslatef(6, 2, 0); 
    glScalef(1, 4, 6); 
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(-6, 2, 0); 
    glScalef(1, 4, 6); 
    glutSolidCube(1); 
    glPopMatrix();
    
    glColor3f(0.6f, 0.3f, 0.1f); 
    glPushMatrix(); 
    glTranslatef(0, 4.5f, 0); 
    glScalef(14, 1, 6); 
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(0, 7.5f, -2.5f); 
    glScalef(14, 4, 1); 
    glutSolidCube(1); 
    glPopMatrix();

    glColor3f(0.2f, 0.2f, 0.2f); 
    glPushMatrix(); 
    glTranslatef(5, 6, -2.5f); 
    glScalef(0.5, 4, 0.5); 
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(-5, 6, -2.5f); 
    glScalef(0.5, 4, 0.5); 
    glutSolidCube(1); 
    glPopMatrix(); 
    glPopMatrix();
}

void StreetFurniture::drawGardenLamp(float x, float y, float z, bool isOn) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.1f, 0.1f, 0.1f); 
    glPushMatrix(); 
    glScalef(1, 15, 1); 
    glutSolidCube(1); 
    glPopMatrix();

    glTranslatef(0, 8, 0); 
    glColor3f(0.2f, 0.2f, 0.2f); 
    glutSolidSphere(1.5, 10, 10);

    if (isOn) {
        glDisable(GL_LIGHTING); 
        glColor3f(1.0f, 1.0f, 0.8f); 
        glutSolidSphere(1.6, 10, 10);

        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        glDepthMask(GL_FALSE);

        glPushMatrix(); 
        glRotatef(90, 1, 0, 0); 
        glColor4f(1.0f, 0.9f, 0.5f, 0.2f); 
        glutSolidCone(10.0, 20.0, 20, 10); 
        glPopMatrix();

        glDepthMask(GL_TRUE); 
        glDisable(GL_BLEND); 
        glEnable(GL_LIGHTING);
    }
    else { 
        glColor3f(0.4f, 0.4f, 0.4f); 
        glutSolidSphere(1.6, 10, 10); 
    } 
    glPopMatrix();
}

void StreetFurniture::drawStreetLampBase(float x, float y, float z) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.1f, 0.1f, 0.1f);
    glScalef(2, 1, 2); 
    glutSolidCube(1);
    glPopMatrix();
}

void StreetFurniture::drawParkingCanopy(float x, float y, float z, float width, float length) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.3f, 0.3f, 0.35f); 

    float poleH = 15.0f;

    glPushMatrix(); 
    glTranslatef(-width / 2 + 1, poleH / 2, -length / 2 + 1); 
    glScalef(1, poleH, 1);
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(width / 2 - 1, poleH / 2, -length / 2 + 1); 
    glScalef(1, poleH, 1);
    glutSolidCube(1); 
    glPopMatrix();

    glColor3f(0.9f, 0.9f, 0.95f);

    glBegin(GL_QUADS); 
    glVertex3f(-width / 2, poleH, -length / 2); 
    glVertex3f(width / 2, poleH, -length / 2); 
    glVertex3f(width / 2, poleH - 3, length / 2);
    glVertex3f(-width / 2, poleH - 3, length / 2); 
    glEnd();

    glPopMatrix();
}

void StreetFurniture::drawCafeSet(float x, float y, float z) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glScalef(3.0f, 3.0f, 3.0f); 
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.9f, 0.9f, 0.9f); 
    glPushMatrix(); 
    glTranslatef(0, 3, 0); 
    glRotatef(-90, 1, 0, 0); 
    glutSolidCone(4, 1, 10, 1); 
    glPopMatrix();

    glColor3f(0.2f, 0.2f, 0.2f); 
    glPushMatrix(); 
    glTranslatef(0, 1.5, 0); 
    glScalef(0.5, 3, 0.5); 
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 12, 0);
    glRotatef(-90, 1, 0, 0);
    glColor3f(0.8f, 0.2f, 0.2f);
    glutSolidCone(8, 4, 8, 1);
    glPopMatrix();

    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix(); 
    glTranslatef(0, 6, 0);
    glScalef(0.3, 12, 0.3);
    glutSolidCube(1);
    glPopMatrix();

    for (int i = 0; i < 4; i++) { 
        glPushMatrix(); 
        glRotatef(i * 90 + 45, 0, 1, 0);
        glTranslatef(0, 0, 5);

        glColor3f(0.4f, 0.2f, 0.1f); 
        glPushMatrix(); 
        glTranslatef(0, 2, 0); 
        glScalef(3, 0.5, 3); 
        glutSolidCube(1); 
        glPopMatrix(); 

        glPushMatrix();
        glTranslatef(0, 4, -1.5); 
        glScalef(3, 4, 0.5); 
        glutSolidCube(1); 
        glPopMatrix(); 
        glPopMatrix(); 
    } 
    glPopMatrix();
}