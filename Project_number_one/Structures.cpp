#include "Structures.h"

void Structures::drawGazebo(float x, float y, float z) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.5f, 0.35f, 0.2f); 
    glPushMatrix(); 
    glScalef(30, 1, 30); 
    glutSolidCube(1); 
    glPopMatrix();

    glColor3f(0.4f, 0.2f, 0.1f); 
    float pOff = 13.0f;
    for (int i = 0; i < 4; i++) {
        glPushMatrix(); 
        float px = (i < 2) ? pOff : -pOff;
        float pz = (i % 2 == 0) ? pOff : -pOff; 
        glTranslatef(px, 10, pz); 
        glScalef(1, 20, 1); 
        glutSolidCube(1); 
        glPopMatrix();
    }

    glColor3f(0.3f, 0.1f, 0.0f); 
    glPushMatrix(); 
    glTranslatef(0, 20, 0); 
    glRotatef(-45, 0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(22, 10, 4, 1);
    glPopMatrix(); 

    glPopMatrix();
}

void Structures::drawWoodenBridge(float x, float y, float z, float length, float width) {
    glPushMatrix(); 

    glTranslatef(x, y, z); 
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.55f, 0.4f, 0.25f); 
    int numPlanks = 40; 
    float plankW = length / numPlanks;
    for (int i = 0; i < numPlanks; i++) {
        float progress = (float)i / numPlanks; 
        float arcH = sin(progress * M_PI) * 8.0f;

        glPushMatrix(); 
        glTranslatef(-length / 2 + i * plankW, arcH, 0); 
        glScalef(plankW * 0.9, 0.5, width);
        glutSolidCube(1); 
        glPopMatrix();

        glColor3f(0.3f, 0.2f, 0.1f); 
        glPushMatrix(); 
        glTranslatef(-length / 2 + i * plankW, arcH + 3, width / 2 - 0.5); 
        glScalef(0.5, 6, 0.5); 
        glutSolidCube(1); 
        glPopMatrix();

        glPushMatrix(); 
        glTranslatef(-length / 2 + i * plankW, arcH + 3, -width / 2 + 0.5); 
        glScalef(0.5, 6, 0.5); 
        glutSolidCube(1); 
        glPopMatrix();

        glColor3f(0.55f, 0.4f, 0.25f);
    } 
    glPopMatrix();
}

void Structures::drawKiosk(float x, float y, float z) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glDisable(GL_TEXTURE_2D);
    
    glColor3f(0.2f, 0.2f, 0.2f); 
    glPushMatrix(); 
    glTranslatef(0, 6, 0); 
    glScalef(15, 12, 10); 
    glutSolidCube(1); 
    glPopMatrix();
    
    glColor3f(0.9f, 0.9f, 0.9f); 
    glPushMatrix(); 
    glTranslatef(0, 6, 5.1f); 
    glScalef(12, 6, 0.1f);
    glutSolidCube(1); 
    glPopMatrix();
    
    glColor3f(0.6f, 0.4f, 0.2f);
    glPushMatrix(); 
    glTranslatef(0, 3, 6.0f);
    glScalef(16, 1, 2); 
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.8f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(0, 12.5f, 0);
    glScalef(16, 1, 12);
    glutSolidCube(1); 
    glPopMatrix(); 

    glPopMatrix();
}

void Structures::drawJuiceStall(float x, float y, float z) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 0.5f, 0.0f); 
    glPushMatrix(); 
    glTranslatef(0, 5, 0); 
    glScalef(12, 10, 12); 
    glutSolidCube(1); 
    glPopMatrix();

    glColor3f(0.0f, 0.8f, 0.0f); 
    glPushMatrix(); 
    glTranslatef(0, 11, 6); 
    glScalef(10, 2, 1); 
    glutSolidCube(1); 
    glPopMatrix();

    glColor3f(1, 1, 0); 
    glPushMatrix(); 
    glTranslatef(-3, 6, 6.5);
    glutSolidTeapot(1); 
    glPopMatrix();

    glColor3f(1, 0, 1);
    glPushMatrix(); 
    glTranslatef(3, 6, 6.5);
    glutSolidTeapot(1);
    glPopMatrix(); 
    
    glPopMatrix();
}

void Structures::drawFence(float w, float l) {
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.2f, 0.2f, 0.2f);
    float poleH = 8.0f;
    float gap = 15.0f;

    for (float i = -w / 2; i <= w / 2; i += gap) {
        glPushMatrix(); 
        glTranslatef(i, poleH / 2, -l / 2); 
        glScalef(1, poleH, 1);
        glutSolidCube(1); 
        glPopMatrix(); 
        if (abs(i) > 40) {
            glPushMatrix(); 
            glTranslatef(i, poleH / 2, l / 2); 
            glScalef(1, poleH, 1); 
            glutSolidCube(1); 
            glPopMatrix();
        }
    }

    for (float i = -l / 2; i <= l / 2; i += gap) {
        glPushMatrix(); 
        glTranslatef(w / 2, poleH / 2, i);
        glScalef(1, poleH, 1); 
        glutSolidCube(1); 
        glPopMatrix();

        glPushMatrix(); 
        glTranslatef(-w / 2, poleH / 2, i); 
        glScalef(1, poleH, 1);
        glutSolidCube(1); 
        glPopMatrix(); 
    }
}

void Structures::drawMainGate() {
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.8f, 0.7f, 0.6f);

    glPushMatrix(); 
    glTranslatef(40, 12, 0);
    glScalef(15, 24, 15); 
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(-40, 12, 0); 
    glScalef(15, 24, 15); 
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 22, 0); 
    glScalef(100, 6, 10);
    glutSolidCube(1); 
    glPopMatrix();

    glColor3f(0.1f, 0.4f, 0.1f);
    glPushMatrix(); 
    glTranslatef(0, 26, 0);
    glScalef(50, 4, 2);
    glutSolidCube(1);
    glPopMatrix();
}

void Structures::drawText3D(const char* text, float x, float y, float z, float scale, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, z);

    float width = 0; 
    for (const char* c = text; *c; c++) 
        width += glutStrokeWidth(GLUT_STROKE_ROMAN, *c);

    glScalef(scale, scale, scale);
    glTranslatef(-width / 2.0f, 0, 0);
    glLineWidth(3.0f);
    for (int i = 0; i < 8; i++) { 
        glPushMatrix(); 
        glTranslatef(0, 0, i * 1.5f);
        if (i == 7) 
            glColor3f(r, g, b); 
        else 
            glColor3f(r * 0.6f, g * 0.6f, b * 0.6f); 
        for (const char* c = text; *c; c++) 
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        glPopMatrix(); 
    }
    glLineWidth(1.0f); 

    glPopMatrix();
}

void Structures::drawShowroomSign(bool isNight) {
    float gateZ = 200.0f; 
    float signY = 100.0f;

    glPushMatrix(); 
    glTranslatef(0, signY, gateZ + 3.0f); 
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.8f, 0.8f, 0.85f); 
    glPushMatrix(); 
    glScalef(140, 20, 2); 
    glutSolidCube(1); 
    glPopMatrix();

    glColor3f(0.05f, 0.05f, 0.1f);
    glPushMatrix(); 
    glTranslatef(0, 0, 0.5f);
    glScalef(135, 17, 1.5f);
    glutSolidCube(1); 
    glPopMatrix();

    if (isNight) { 
        glDisable(GL_LIGHTING);
        drawText3D("CARS SHOWROOM", 0, -3.0f, 4.0f, 0.12f, 0.2f, 0.8f, 1.0f); 
        glEnable(GL_LIGHTING);
    }
    else { 
        drawText3D("CARS SHOWROOM", 0, -3.0f, 4.0f, 0.12f, 0.0f, 0.0f, 0.8f);
    }
    if (isNight)
        glDisable(GL_LIGHTING);

    glColor3f(0.0f, 0.4f, 1.0f);
    glPushMatrix();
    glTranslatef(0, -7.0f, 3.0f);
    glScalef(130, 0.5f, 0.5f);
    glutSolidCube(1); 
    glPopMatrix();

    if (isNight) 
        glEnable(GL_LIGHTING);
    glPopMatrix();
}

void Structures::drawBalloon(float x, float y, float z, float r, float g, float b) {
    glPushMatrix(); 
    glTranslatef(x, y, z);
    glDisable(GL_TEXTURE_2D);

    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, -10, 0);
    glEnd();

    glColor3f(r, g, b);
    glScalef(1, 1.2, 1); 
    glutSolidSphere(2, 10, 10);
    glPopMatrix();
}

void Structures::drawGrandOpeningDecor(float startZ, float endZ) {
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.8f, 0.0f, 0.0f);
    float carpetY = 0.15f;

    glBegin(GL_QUADS); 
    glNormal3f(0, 1, 0);

    glVertex3f(-15, carpetY, startZ);
    glVertex3f(15, carpetY, startZ);

    glVertex3f(15, carpetY, endZ);
    glVertex3f(-15, carpetY, endZ);
    glEnd();
    for (int i = 0; i < 6; i++) {
        float z = startZ + 10 + i * 12;
        drawBalloon(20, 15 + (i % 2) * 2, z, 1, 0, 0);
        drawBalloon(22, 12 + (i % 2) * 2, z, 1, 1, 0);
        drawBalloon(-20, 15 + (i % 2) * 2, z, 1, 0, 0);
        drawBalloon(-22, 12 + (i % 2) * 2, z, 1, 1, 0);
    }
}