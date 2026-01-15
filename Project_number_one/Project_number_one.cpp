#include <GL/glut.h>
#include <vector>
#include <cmath>
#include "Pillar.h"
#include "Hpillar.h"
#include "Showroomside.h"
#include "ShowroomGate.h"
#include "GlassWindow.h"
#include "PlusRing3D.h"



using namespace std;

PlusRing3D symbol;
vector<Pillar> pillars;
vector<Hpillar> hpillars;
vector<Showroomside> showroomsides;
vector<GlassWindow> glasswindows;
int maxX = 40, maxz = 50, diff = 16;
int minX = -maxX, minz = -maxz;
pair<float, float> p[4] = { {maxX,maxz},{minX,maxz},{maxX,minz},{minX,minz} };
int h = 30;


float camX = 0.0f, camY = 5.0f, camZ = 20.0f;
float lookX = 0.0f, lookY = 0.0f, lookZ = -1.0f;
float yaw = -90.0f, pitch = 0.0f;
int lastMouseX, lastMouseY;
bool firstMouse = true;
#define M_PI acos(-1)

void updateLookVector() {
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
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
}

void initEnvironment() {
    
    for (int i=0;i<4;i++)
    pillars.push_back(Pillar(p[i].first, 0, p[i].second, h));
    pillars.push_back(Pillar(0,0, 0, h,0.2));
    hpillars.push_back(Hpillar(maxX, 0, 0, 0.4, 0.8, maxz-minz));
    hpillars.push_back(Hpillar(maxX, h-0.4, 0, 0.4, 0.8, maxz - minz ));
    hpillars.push_back(Hpillar(minX, 0, 0, 0.4, 0.8, maxz - minz ));
    hpillars.push_back(Hpillar(minX, h - 0.4, 0, 0.4, 0.8, maxz - minz ));
    hpillars.push_back(Hpillar(0, 0, minz, 0.4, maxX-minX, 0.8));
    hpillars.push_back(Hpillar(0, h - 0.4, minz, 0.4, maxX - minX, 0.8));
    hpillars.push_back(Hpillar(0, h - 0.4, maxz, 0.4, maxX - minX, 0.8));
    hpillars.push_back(Hpillar(minX+ (maxX - minX - diff) / 4, 0, maxz, 0.4, (maxX-minX-diff)/2, 0.8));
    hpillars.push_back(Hpillar(maxX- (maxX - minX - diff) / 4, 0, maxz, 0.4, (maxX - minX  - diff)/2, 0.8));

    // ground line
    hpillars.push_back(Hpillar(maxX - (maxX - minX - diff) / 2,0,0,0.4,0.8,maxz-minz-2*diff));
    hpillars.push_back(Hpillar(minX + (maxX - minX - diff) / 2, 0, 0, 0.4, 0.8, maxz - minz - 2*diff));
    
    // roof line
    hpillars.push_back(Hpillar(maxX - (maxX - minX - diff) / 2, h, 0, 0.4, 0.8, maxz - minz - 2 * diff));
    hpillars.push_back(Hpillar(minX + (maxX - minX - diff) / 2, h, 0, 0.4, 0.8, maxz - minz - 2 * diff));


    showroomsides.push_back(Showroomside(maxX, minX, minz, minz, h, 0.2, 0.2,0.2, 12,2));
    showroomsides.push_back(Showroomside(minX, minX, maxz, minz, h, 0.2, 0.2, 0.2, 12, 2));
    showroomsides.push_back(Showroomside(maxX, maxX, maxz, minz, h, 0.2, 0.2, 0.2, 12, 2));
    showroomsides.push_back(Showroomside(minX + (maxX - minX - diff) / 2, minX, maxz, maxz, h, 0.2, 0.2, 0.2, 5,2));
    showroomsides.push_back(Showroomside(maxX,maxX - (maxX - minX - diff) / 2, maxz, maxz, h, 0.2, 0.2, 0.2, 5,2));

    glasswindows.push_back(GlassWindow(maxX, 0, minz, maxX, h, minz, minX, h, minz, minX, 0, minz));
    glasswindows.push_back(GlassWindow(maxX, 0, minz, maxX, h, minz, maxX, h, maxz, maxX, 0, maxz));
    glasswindows.push_back(GlassWindow(minX, 0, minz, minX, h, minz, minX, h, maxz, minX, 0, maxz));
    glasswindows.push_back(GlassWindow(minX, 0, maxz, minX, h, maxz, minX + (maxX - minX - diff) / 2 + 1, h, maxz, minX + (maxX - minX - diff) / 2 + 1, 0, maxz));
    glasswindows.push_back(GlassWindow(maxX, 0, maxz, maxX, h, maxz, maxX - (maxX - minX - diff) / 2 - 1, h, maxz, maxX - (maxX - minX - diff) / 2 - 1, 0, maxz));

}

void handleKeypress(unsigned char key, int x, int y) {
    float speed = 0.6f;
    switch (key) {
    case 'w': camX += lookX * speed; camZ += lookZ * speed; break;
    case 's': camX -= lookX * speed; camZ -= lookZ * speed; break;
    case 'a': camX -= (lookZ * -1.0f) * speed; camZ -= lookX * speed; break;
    case 'd': camX += (lookZ * -1.0f) * speed; camZ += lookX * speed; break;
    case 'q': camY -= 0.5; break;
    case 'e': camY += 0.5; break;
    case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void handlePassiveMouse(int x, int y) {
    if (firstMouse) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
    }

    float xOffset = x - lastMouseX;
    float yOffset = lastMouseY - y;
    lastMouseX = x;
    lastMouseY = y;

    float sensitivity = 0.1f;
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;

    updateLookVector();
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camX, camY, camZ,
        camX + lookX, camY + lookY, camZ + lookZ,
        0.0f, 1.0f, 0.0f);

    for (const auto& p : pillars) {
        p.draw();
    }

    for (const auto& p : hpillars) {
        p.draw();
    }

    for (const auto& p : showroomsides) {
        p.draw();
    }

    for (const auto& p : glasswindows) {
        p.draw();
    }

    float curz = (maxz - minz - 2 * diff) / 2, curx = maxX - (maxX - minX - diff) / 2,cury=0,base;

    base = curz;
    while (cury < h)
    {
        curz = base;
        for (curz; curz > -(maxz - minz - 2 * diff) / 2; curz -= 3) {
            glPushMatrix();
            glTranslatef(curx, cury+1.5, curz-1.5);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            symbol.draw();
            glPopMatrix();
        }
        cury += 3;
    }

    ShowroomGate showroomgate;
    symbol.draw();
    showroomgate.draw();

    glutSwapBuffers();
}

void handleResize(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Architectural Walkthrough");

    initRendering();
    initEnvironment();
    updateLookVector();

    glutDisplayFunc(display);
    glutReshapeFunc(handleResize);
    glutKeyboardFunc(handleKeypress);
    glutPassiveMotionFunc(handlePassiveMouse);

    // glutSetCursor(GLUT_CURSOR_NONE); 

    glutMainLoop();
    return 0;
}

/*
2 3 5
2 2 2 2 3
*/