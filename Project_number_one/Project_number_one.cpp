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
#include "lake.h"

#define M_PI acos(-1)

using namespace std;

// Camera
float camX = 500, camY = 300, camZ = 500.0f;
float lookX = -1.0f, lookY = -1.0f, lookZ = -1.0f;
float yaw = -90.0f, pitch = 0.0f;
int lastMouseX, lastMouseY;
bool firstMouse = true, ignoreWarp;

float maxX = 150, maxz = 200, diff = 45;
float minX = -maxX, minz = -maxz;
Maherheader maher;
AbrarCode abrarCode;
GlassWindow lake = GlassWindow(minX-170, -0.2, maxz + 100, minX-170, -0.2, maxz + 600, minX + 140, -0.2, maxz + 600, minX + 170, -0.2, maxz + 100);

void updateLookVector() {
    // التعديل: السماح بتحديث النظر إلا أثناء أنيميشن الدخول فقط
    if (abrarCode.getState() == STATE_ENTERING) return;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    float radYaw = yaw * M_PI / 180.0f;
    float radPitch = pitch * M_PI / 180.0f;
    lookX = cos(radPitch) * cos(radYaw);
    lookY = sin(radPitch);
    lookZ = cos(radPitch) * sin(radYaw);
}

void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);
    glEnable(GL_LIGHT6);
    glEnable(GL_LIGHT7);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat globalAmbient[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    GLfloat diffuse0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specular0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
}

void initEnvironment() {}

void handleKeypress(unsigned char key, int x, int y) {
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
    // التعديل: السماح بالماوس أثناء القيادة
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

    // في وضع القيادة، الكاميرا تتحرك، لذا يجب تحديث متجه النظر
    if (abrarCode.getState() != STATE_WALKING) {
        updateLookVector();
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ,
        camX + lookX, camY + lookY, camZ + lookZ,
        0.0f, 1.0f, 0.0f);
        
    abrarCode.drawCars();
    maher.draw(camX,camY,camZ);
    glPushMatrix();
    glTranslatef(0,-1,0);
    glScalef(1, -1, 1);
    abrarCode.draw4Cars();
    maher.draw(camX, camY, camZ);
    glPopMatrix();
    glEnd();

    lake.setAlpha(0.3);
    lake.draw();
    glutSwapBuffers();
}

void handleResize(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // zNear = 0.1 لمنع تشوه الأرضية
    gluPerspective(45.0, (float)w / (float)h, 0.1, 2000.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Showroom Simulator");

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
