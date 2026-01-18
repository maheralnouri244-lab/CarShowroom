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
#include "PersistentCloud.h"
#include <ctime>
#include "RainSystem.h"
#include "SaraCode.h"

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
int lastMouseX, lastMouseY, weatherstatus = 0;
const int CloudCount = 20;
bool firstMouse = true, ignoreWarp;

float maxX = 150, maxz = 200, diff = 45;
float minX = -maxX, minz = -maxz;

// Instances
Maherheader maher;
AbrarCode abrarCode;
Jeep_Builder_Final myJeep;
SaraCode saraCode;
GlassWindow lake = GlassWindow(minX - 170, -0.2, maxz + 100, minX - 170, -0.2, maxz + 600, minX + 140, -0.2, maxz + 600, minX + 170, -0.2, maxz + 100);


const float SHOWROOM_HEIGHT = 60.0f;
const float GATE_WIDTH = 45.0f;
const float GATE_MIN_X = -GATE_WIDTH / 2.0f;
const float GATE_MAX_X = GATE_WIDTH / 2.0f;
const float PLAYER_BUFFER_Y = 5.0f;
const float COLLISION_BUFFER = 3.0f;
const float GLOBAL_GROUND_LEVEL = 2.0f;


struct BoundingBox {
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
};

std::vector<BoundingBox> collidableObjects;

bool isStrictlyInside(float x, float z) {
    return (x > minX && x < maxX && z > minz && z < maxz);
}
void createCollidables() {
    collidableObjects.clear();

    // 1. جدران المعرض (لا تغيريها)
    float wall_1_x = minX + (maxX - minX - diff) / 2.0f;
    float wall_2_x = maxX - (maxX - minX - diff) / 2.0f;
    float wall_z_range = (maxz - minz - 2 * diff) / 2.0f;

    float wallBuffer = 5.0f; // المسافة التي سيتوقف عندها اللاعب قبل الجدار

    // الجدران الجانبية مع Buffer (زدنا السماكة من 2.0 إلى wallBuffer)
    collidableObjects.push_back({ wall_1_x - wallBuffer, 0.0f, -wall_z_range, wall_1_x + wallBuffer, SHOWROOM_HEIGHT, wall_z_range });
    collidableObjects.push_back({ wall_2_x - wallBuffer, 0.0f, -wall_z_range, wall_2_x + wallBuffer, SHOWROOM_HEIGHT, wall_z_range });

    // إضافة جدران إضافية (الخلفي مثلاً) إذا أردتِ حماية كاملة:
    // الجدار الخلفي (عند minz)
    collidableObjects.push_back({ minX, 0.0f, minz - wallBuffer, maxX, SHOWROOM_HEIGHT, minz + wallBuffer });

    // الجدار الأمامي (عند maxz) باستثناء منطقة البوابة
    collidableObjects.push_back({ minX, 0.0f, maxz - wallBuffer, GATE_MIN_X, SHOWROOM_HEIGHT, maxz + wallBuffer }); // يمين البوابة
    collidableObjects.push_back({ GATE_MAX_X, 0.0f, maxz - wallBuffer, maxX, SHOWROOM_HEIGHT, maxz + wallBuffer }); // يسار البوابة



    float extentX = (40.0f / 2.0f) * 3.0f; // العرض الكلي للمنصة
    float extentZ = (40.0f / 2.0f) * 1.0f; // سمك المنصة
    float center_x_C1 = -minX - (maxX - minX - diff) / 4.0f;
    float center_x_C2 = minX + (maxX - minX - diff) / 4.0f;

    // صندوق المنصة الأولى والثانية
    collidableObjects.push_back({ center_x_C1 - extentX, 0.0f, -extentZ, maxX, 30.0f, extentZ });
    collidableObjects.push_back({ minX, 0.0f, -extentZ, center_x_C2 + extentX, 30.0f, extentZ });
    // 2. سيارات أبرار (Beetle) - جهة اليمين
    float beetle_x = 90.0f;
    float beetle_z[] = { -40.0f, -75.0f, -110.0f, -145.0f };
    for (float z : beetle_z) {
        collidableObjects.push_back({ beetle_x - 18.0f, 0.0f, z - 10.0f, beetle_x + 18.0f, 15.0f, z + 10.0f });
    }

    float myCarX = -110.0f;
    float myCarZ[] = { 160.0f, 120.0f, 80.0f, 40.0f };

    // التعديل هنا:
    // قللنا نصف الطول (Z) لفتح ممر واسع بين السيارات
    // زدنا مسافة الأمان الجانبية (X) لكي لا تلتصقي بالباب
    float carBoxWidth = 10.0f;  // العرض الجانبي
    float carBoxLength = 10.0f; // جعلنا طول الصندوق صغيراً جداً (فقط في منتصف السيارة) 
    // لكي يفتح ممر واسع أمام وخلف كل سيارة

    float sideSafety = 3.0f;    // مسافة أمان من جهة الأبواب

    for (float zPos : myCarZ) {
        collidableObjects.push_back({
            myCarX - (carBoxWidth + sideSafety), 0.0f, zPos - carBoxLength,
            myCarX + (carBoxWidth + sideSafety), 20.0f, zPos + carBoxLength
            });
    }

    // 4. سيارات الجيب (Jeep) - السماح بالمرور بين السيارات
    float jeep_x = 120.0f;
    float jeep_safety_X = 5.0f;  // مسافة أمان عرضية (للتوقف بعيداً عن الباب)
    float jeep_box_Z = 12.0f;    // صغرنا الطول لترك فراغ للمشي بين كل سيارة وسيارة

    for (int i = 1; i <= 3; i++) {
        float jZ = 50.0f * i;
        collidableObjects.push_back({
            jeep_x - (15.0f + jeep_safety_X), 0.0f, jZ - jeep_box_Z,
            jeep_x + (15.0f + jeep_safety_X), 25.0f, jZ + jeep_box_Z
            });
    }
}

PersistentCloud myClouds[CloudCount];
RainSystem myRain(1000);

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
        float nextX = camX, nextY = camY, nextZ = camZ;

        switch (key) {
        case 'w': nextX += lookX * speed; nextZ += lookZ * speed; break;
        case 's': nextX -= lookX * speed; nextZ -= lookZ * speed; break;
        case 'a': nextX -= (lookZ * -1.0f) * speed; nextZ -= lookX * speed; break;
        case 'd': nextX += (lookZ * -1.0f) * speed; nextZ += lookX * speed; break;
        case 'q': nextY -= 10.0f; break;
        case 'e': nextY += 10.0f; break;
        case 'g': case 'f': abrarCode.handleInput(key, camX, camZ); glutPostRedisplay(); return;
        case 'r':   weatherstatus = (weatherstatus + 1) % 3; break;
        case 27: exit(0); return;
        }

        if (nextY < GLOBAL_GROUND_LEVEL) {
            nextY = GLOBAL_GROUND_LEVEL;
        }
        saraCode.handleInput(key);
        bool isOverShowroom = isStrictlyInside(nextX, nextZ);

        if (isOverShowroom) {
            float roofMargin = 2.0f;

            if (camY >= SHOWROOM_HEIGHT && nextY < (SHOWROOM_HEIGHT + roofMargin)) {
                nextY = SHOWROOM_HEIGHT + roofMargin;
            }
            else if (camY < SHOWROOM_HEIGHT && nextY >(SHOWROOM_HEIGHT - roofMargin)) {
                nextY = SHOWROOM_HEIGHT - roofMargin;
            }

            if (nextY < PLAYER_BUFFER_Y) {
                nextY = PLAYER_BUFFER_Y;
            }
        }

        camY = nextY;

        bool isCurrentlyInside = isStrictlyInside(camX, camZ);
        bool willBeInside = isStrictlyInside(nextX, nextZ);


        if (camY < SHOWROOM_HEIGHT) {
            if ((!isCurrentlyInside && willBeInside) || (isCurrentlyInside && !willBeInside)) {

                // فحص: هل اللاعب ضمن عرض البوابة؟
                bool inGateWidth = (nextX > GATE_MIN_X && nextX < GATE_MAX_X);

                // فحص: هل اللاعب تحت مستوى الحائط العلوي؟ (الحائط يبدأ من ارتفاع 30)
                bool isLowEnough = (camY < 30.0f);

                // إذا كان خارج العرض "أو" مرتفع زيادة (رح يخبط بالحيط)، امنعي الحركة
                if (!inGateWidth || !isLowEnough) {
                    glutPostRedisplay();
                    return;
                }
            }
        }

        if (willBeInside && camY < SHOWROOM_HEIGHT) {
            BoundingBox playerBox = { nextX - 1.5f, camY - PLAYER_BUFFER_Y, nextZ - 1.5f, nextX + 1.5f, camY, nextZ + 1.5f };
            for (const auto& objBox : collidableObjects) {
                if (playerBox.maxX > objBox.minX && playerBox.minX < objBox.maxX &&
                    playerBox.maxY > objBox.minY && playerBox.minY < objBox.maxY &&
                    playerBox.maxZ > objBox.minZ && playerBox.minZ < objBox.maxZ)
                {
                    glutPostRedisplay(); return;
                }
            }
        }

        camX = nextX;
        camZ = nextZ;
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
    myRain.update();
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

    glEnable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camX, camY, camZ,
        camX + lookX, camY + lookY, camZ + lookZ,
        0.0f, 1.0f, 0.0f);

    drawSkyBody(isLightOn);


    if (isLightOn) {
        glEnable(GL_LIGHT0);
        GLfloat ambientDay[] = { 0.4f, 0.4f, 0.4f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientDay);
    }
    else {
        glDisable(GL_LIGHT0);
        GLfloat ambientNight[] = { 0.3f, 0.3f, 0.5f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientNight);
    }

    if (weatherstatus == 2)
        myRain.draw();
    abrarCode.drawCars();
    saraCode.drawAll();
    // =======================================================
    // (The Corrected Isolation Protocol) - Jeep Section
    // =======================================================
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // 1. تفعيل خاصية تتبع الألوان (هذا هو الحل لمشكلة اللون الرمادي)
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat ambientDay[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat ambientNight[] = { 0.01f, 0.01f, 0.01f, 0.5f };

    // نستخدم مؤشر ليؤشر على المصفوفة المناسبة حسب الحالة
    GLfloat* currentAmbient;

    if (isLightOn) {
        currentAmbient = ambientDay;
    }
    else {
        currentAmbient = ambientNight;
    }    // 2. ضبط إضاءة محيطة معتدلة (0.3 تجعل الألوان تظهر بوضوح دون أن تصبح بيضاء)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, currentAmbient);

    // 3. منع انعكاس الضوء القوي الذي قد يسبب بقعاً بيضاء (Specular)
    GLfloat noSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, noSpecular);

    float x_jeep = 120.0f;
    float y_jeep = 0.0f;
    float z_jeep = 50.0f;
    float size_jeep = 6.0f;
    float angle = 90.0f;

    for (int i = 1; i <= 3; i++) {
        myJeep.drawJeep(x_jeep, y_jeep, z_jeep * i, size_jeep, angle);
    }

    glPopAttrib();

    for (int i = 0; weatherstatus > 0 && i < CloudCount; i++) {
        myClouds[i].draw(weatherstatus);
    }

    maher.draw(camX, camY, camZ, isLightOn);

    glPushMatrix();
    glTranslatef(0, -1, 0);
    glScalef(1, -1, 1);
    if (weatherstatus == 2)
        myRain.draw();
    abrarCode.drawCars();
    drawSkyBody(isLightOn);

    for (int i = 0; weatherstatus > 0 && i < CloudCount; i++) {
        myClouds[i].draw(weatherstatus);
    }

    maher.draw(camX, camY, camZ, isLightOn);
    glPopMatrix();
    lake.setAlpha(0.7);
    lake.draw();

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
    srand(time(0));
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Showroom Simulator - Lighting Edition"); // Title Update

    initRendering();
    createCollidables();
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