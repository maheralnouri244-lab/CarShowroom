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

#define M_PI acos(-1)

using namespace std;

float camX = 500, camY = 300, camZ = 500.0f;
float lookX = -1.0f, lookY = -1.0f, lookZ = -1.0f;
float yaw = -90.0f, pitch = 0.0f;
int lastMouseX, lastMouseY;
bool firstMouse = true, ignoreWarp;

Maherheader maher;
AbrarCode abrarCode;
Jeep_Builder_Final myJeep;

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
    myJeep.init();
}

void initEnvironment() {}

void handleKeypress(unsigned char key, int x, int y) {
    float speed = 3.0f;
    switch (key) {
    case 'w': camX += lookX * speed; camZ += lookZ * speed; break;
    case 's': camX -= lookX * speed; camZ -= lookZ * speed; break;
    case 'a': camX -= (lookZ * -1.0f) * speed; camZ -= lookX * speed; break;
    case 'd': camX += (lookZ * -1.0f) * speed; camZ += lookX * speed; break;
    case 'q': camY -= 1.0; break;
    case 'e': camY += 1.0; break;
    case 'f': abrarCode.interact(); break;
    case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void handlePassiveMouse(int x, int y) {
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
    abrarCode.update();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ,
        camX + lookX, camY + lookY, camZ + lookZ,
        0.0f, 1.0f, 0.0f);

    abrarCode.draw4Cars();
    maher.draw();;

    // =======================================================
    // بروتوكول العزل الشامل (The Full Isolation Protocol)
    // =======================================================

    // 1. حفظ كل إعدادات المعرض (الإضاءة، التكستشر، الألوان)
    // GL_TEXTURE_BIT: هو الإضافة الجديدة المهمة لحفظ حالة الصور
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT);

    // 2. الخطوة الحاسمة: إيقاف التكستشر!
    // هذا سيمنع صور المعرض من تغطية لون السيارة
    glDisable(GL_TEXTURE_2D);

    // 3. إيقاف تلوين المواد المباشر (للسماح للسيارة باستخدام خاماتها المعدنية)
    glDisable(GL_COLOR_MATERIAL);

    // 4. إصلاح الإضاءة (إصلاح مشكلة الأبعاد المسطحة)
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);  // التأكد من أن الإضاءة تعمل

    // 5. تقليل ضجيج الإضاءة (إبقاء ضوء واحد فقط للسيارة)
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);

    // 6. رسم السيارات في المواقع المطلوبة
    // بما أننا أطفأنا التكستشر، ستظهر الألوان السوداء والمرايا الآن
    float x_jeep = 120.0f;
    float y_jeep = 0.0f;
    float z_jeep = 50.0f;
    float size_jeep = 6.0f;
    for (int i = 1; i <= 3; i++) {
        myJeep.drawJeep(x_jeep, y_jeep, z_jeep * i, size_jeep);
    }


    // 7. استعادة حالة المعرض (يعيد تشغيل التكستشر والإضاءة لباقي المشهد)
    glPopAttrib();

    // =======================================================

    glutSwapBuffers();
}

void handleResize(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 1000.0);
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
    glutTimerFunc(16, update, 0);

    glutSetCursor(GLUT_CURSOR_NONE);
    glutMainLoop();
    return 0;
}