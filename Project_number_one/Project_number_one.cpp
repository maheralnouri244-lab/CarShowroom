#include <GL/glut.h>
#include <vector>
#include <cmath>
#include "Pillar.h"
#include "Hpillar.h"
#include "Showroomside.h"
#include "ShowroomGate.h"
#include "GlassWindow.h"
#include "PlusRing3D.h"
#include "Maherheader.h"
#include "AbrarCode.h"
#include "JeepAsset.h"
#include "lake.h"

#define M_PI acos(-1)

using namespace std;

// ==========================================
// (Lighting Control)
// ==========================================
bool isLightOn = false; 

// Camera Variables
float camX = 500, camY = 300, camZ = 500.0f;
float lookX = -1.0f, lookY = -1.0f, lookZ = -1.0f;
float yaw = -90.0f, pitch = 0.0f;
int lastMouseX, lastMouseY;
bool firstMouse = true, ignoreWarp;

float maxX = 150, maxz = 200, diff = 45;
float minX = -maxX, minz = -maxz;

// Instances
Maherheader maher;
AbrarCode abrarCode;
Jeep_Builder_Final myJeep;
GlassWindow lake = GlassWindow(minX - 170, -0.2, maxz + 100, minX - 170, -0.2, maxz + 600, minX + 140, -0.2, maxz + 600, minX + 170, -0.2, maxz + 100);

void updateLookVector() {
    if (abrarCode.getState() == STATE_ENTERING) return;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    float radYaw = yaw * M_PI / 180.0f;
    float radPitch = pitch * M_PI / 180.0f;
    lookX = cos(radPitch) * cos(radYaw);
    lookY = sin(radPitch);
    lookZ = cos(radPitch) * sin(radYaw);
}

// ==========================================
// (Architectural Lighting Setup)
// ==========================================
void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glDisable(GL_LIGHT1);


    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    GLfloat lightPos[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    myJeep.init();
}

void drawSkyBody(bool isDay) {
    glPushMatrix();

    glTranslatef(600.0f, 600.0f, 600.0f);


    glDisable(GL_LIGHTING);

    if (isDay) {
        glColor3f(1.0f, 0.9f, 0.0f);
        glutSolidSphere(40.0, 20, 20);
        glEnable(GL_BLEND);
        glColor4f(1.0f, 0.8f, 0.0f, 0.3f);
        glutSolidSphere(55.0, 20, 20);
        glDisable(GL_BLEND);
    }
    else {
        glColor3f(0.9f, 0.9f, 0.9f);
        glutSolidSphere(25.0, 20, 20);
    }
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void initEnvironment() {}

void handleKeypress(unsigned char key, int x, int y) {

    if (key == 'l' || key == 'L') {
        isLightOn = !isLightOn; 
        glutPostRedisplay();
        return;
    }

    if (abrarCode.getState() == STATE_WALKING) {
        float speed = 3.0f;
        switch (key) {
        case 'w': camX += lookX * speed; camZ += lookZ * speed; break;
        case 's': camX -= lookX * speed; camZ -= lookZ * speed; break;
        case 'a': camX -= (lookZ * -1.0f) * speed; camZ -= lookX * speed; break;
        case 'd': camX += (lookZ * -1.0f) * speed; camZ += lookX * speed; break;
        case 'q': camY -= 1.0; break;
        case 'e': camY += 1.0; break;
        case 'g': case 'f': abrarCode.handleInput(key, camX, camZ); break;
        case 27: exit(0); break;
        }
    }
    else {
        abrarCode.handleInput(key, camX, camZ);
        if (key == 27) exit(0);
    }
    glutPostRedisplay();
}

void handlePassiveMouse(int x, int y) {
    if (abrarCode.getState() == STATE_ENTERING) return;

    if (ignoreWarp) {
        ignoreWarp = false;
        return;
    }
    int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    float xOffset = x - centerX;
    float yOffset = centerY - y;
    float sensitivity = 0.1f;
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    updateLookVector();
    ignoreWarp = true;
    glutWarpPointer(centerX, centerY);
    glutPostRedisplay();
}

void update(int value) {
    abrarCode.update(camX, camY, camZ, yaw, pitch);

    if (abrarCode.getState() != STATE_WALKING) {
        updateLookVector();
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void display() {
    // ==========================================
    // 1. (Background System)
    // ==========================================
    if (isLightOn) {

        glClearColor(0.52f, 0.80f, 0.92f, 1.0f);
    }
    else {
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camX, camY, camZ,
        camX + lookX, camY + lookY, camZ + lookZ,
        0.0f, 1.0f, 0.0f);

    drawSkyBody(isLightOn);


    if (isLightOn) {
        glEnable(GL_LIGHTING);
    }
    else {

        glEnable(GL_LIGHTING);

        glDisable(GL_LIGHTING);
    }

    abrarCode.drawCars();
    maher.draw(camX, camY, camZ);


    glPushMatrix();
    glTranslatef(0, -1, 0);
    glScalef(1, -1, 1);
    abrarCode.drawCars();
    maher.draw(camX, camY, camZ);
    glPopMatrix();

    lake.setAlpha(0.3);
    lake.draw();

    // =======================================================
    // (The Full Isolation Protocol)
    // =======================================================
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    if (isLightOn) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }
    else {
        glDisable(GL_LIGHTING);
    }

    glDisable(GL_LIGHT1); glDisable(GL_LIGHT2); glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4); glDisable(GL_LIGHT5); glDisable(GL_LIGHT6); glDisable(GL_LIGHT7);

    float x_jeep = 120.0f;
    float y_jeep = 0.0f;
    float z_jeep = 50.0f;
    float size_jeep = 6.0f;

    for (int i = 1; i <= 3; i++) {
        myJeep.drawJeep(x_jeep, y_jeep, z_jeep * i, size_jeep);
    }

    glPopAttrib();
    glutSwapBuffers();
}

void handleResize(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 2000.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Showroom Simulator - Lighting Edition"); // Title Update

    initRendering();
    initEnvironment();
    updateLookVector();

    glutDisplayFunc(display);
    glutReshapeFunc(handleResize);
    glutKeyboardFunc(handleKeypress);
    glutPassiveMotionFunc(handlePassiveMouse);
    glutTimerFunc(16, update, 0);

    glutSetCursor(GLUT_CURSOR_NONE);
    glutMainLoop();
    return 0;
}