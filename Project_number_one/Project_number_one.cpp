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
int lastMouseX, lastMouseY, weatherstatus=0;
const int CloudCount = 20;
bool firstMouse = true, ignoreWarp;

float maxX = 150, maxz = 200, diff = 45;
float minX = -maxX, minz = -maxz;

// Instances
Maherheader maher;
AbrarCode abrarCode;
Jeep_Builder_Final myJeep;
GlassWindow lake = GlassWindow(minX - 170, -0.2, maxz + 100, minX - 170, -0.2, maxz + 600, minX + 140, -0.2, maxz + 600, minX + 170, -0.2, maxz + 100);

// --- نظام الاصطدام ---
const float SHOWROOM_HEIGHT = 60.0f;
const float GATE_WIDTH = 45.0f;
const float GATE_MIN_X = -GATE_WIDTH / 2.0f;
const float GATE_MAX_X = GATE_WIDTH / 2.0f;
const float PLAYER_BUFFER_Y = 5.0f;
const float COLLISION_BUFFER = 3.0f;
const float GLOBAL_GROUND_LEVEL = 2.0f; 

// هيكل لتمثيل صندوق الاصطدام
struct BoundingBox {
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
};

// قائمة تحتوي على كل الأجسام القابلة للاصطدام
std::vector<BoundingBox> collidableObjects;

// دالة مساعدة للتحقق من وجود الكاميرا داخل حدود المعرض
bool isStrictlyInside(float x, float z) {
    // استخدم المتغيرات minX, maxX, minz, maxz الموجودة لديك
    return (x > minX && x < maxX && z > minz && z < maxz);
}
void createCollidables() {
    // 1. الجدران الداخلية
    float wall_1_x = minX + (maxX - minX - diff) / 2.0f;
    float wall_2_x = maxX - (maxX - minX - diff) / 2.0f;
    float wall_z_range = (maxz - minz - 2 * diff) / 2.0f;
    collidableObjects.push_back({ wall_1_x - 2.0f, 0.0f, -wall_z_range, wall_1_x + 2.0f, SHOWROOM_HEIGHT, wall_z_range });
    collidableObjects.push_back({ wall_2_x - 2.0f, 0.0f, -wall_z_range, wall_2_x + 2.0f, SHOWROOM_HEIGHT, wall_z_range });

    // 2. الفواصل البيضاوية (التي تفصل الأجنحة)
    float center_x_C1 = -minX - (maxX - minX - diff) / 4.0f;
    float center_x_C2 = minX + (maxX - minX - diff) / 4.0f;
    float extentX = (40.0f / 2.0f) * 3.0f;
    float extentZ = (40.0f / 2.0f) * 1.0f;
    collidableObjects.push_back({ center_x_C1 - extentX, 0.0f, -extentZ, maxX, 30.0f, extentZ });
    collidableObjects.push_back({ minX, 0.0f, -extentZ, center_x_C2 + extentX, 30.0f, extentZ });

    // 3. سيارات Beetle
    float car_center_x = 90.0f; // تم تحديث الموقع
    float car_y_pos = 2.0f;
    float car_extent_x = 18.4f;
    float car_extent_z = 7.2f;
    float beetle_z_pos[] = { -40.0f, -75.0f, -110.0f, -145.0f };
    for (float z_pos : beetle_z_pos) {
        collidableObjects.push_back({
            car_center_x - car_extent_x, car_y_pos, z_pos - car_extent_z,
            car_center_x + car_extent_x, car_y_pos + 12.0f, z_pos + car_extent_z
            });
    }

    // 4. سيارات Jeep
    float jeep_x = 120.0f, jeep_y = 0.0f, jeep_z_base = 50.0f, jeep_size = 6.0f;
    float jeep_len = 6.3f * jeep_size;
    float jeep_width = 2.8f * jeep_size;
    float jeep_height = 3.25f * jeep_size;
    float jeep_extent_x = jeep_width / 2.0f;
    float jeep_extent_z = jeep_len / 2.0f;
    for (int i = 1; i <= 3; i++) {
        float current_z = jeep_z_base * i;
        collidableObjects.push_back({
            jeep_x - jeep_extent_x, jeep_y, current_z - jeep_extent_z,
            jeep_x + jeep_extent_x, jeep_y + jeep_height, current_z + jeep_extent_z
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

// --- (الدالة الكاملة والمصححة) ---
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
        case 'r':   weatherstatus=(weatherstatus+1)%3; break;
        case 27: exit(0); return;
        }

        // --- 1. منطق الاصطدام العمودي ---
        if (nextY < GLOBAL_GROUND_LEVEL) {
            nextY = GLOBAL_GROUND_LEVEL;
        }
        bool isOverShowroom = isStrictlyInside(nextX, nextZ);
        if (isOverShowroom) {
            if (camY < SHOWROOM_HEIGHT && nextY >= SHOWROOM_HEIGHT) {
                nextY = SHOWROOM_HEIGHT;
            }
            else if (camY >= SHOWROOM_HEIGHT && nextY < SHOWROOM_HEIGHT) {
                nextY = SHOWROOM_HEIGHT;
            }
            if (nextY < PLAYER_BUFFER_Y) {
                nextY = PLAYER_BUFFER_Y;
            }
        }
        camY = nextY;

        // --- 2. منطق الاصطدام الأفقي ---
        bool isCurrentlyInside = isStrictlyInside(camX, camZ);
        bool willBeInside = isStrictlyInside(nextX, nextZ);
        bool landedOnRoof = (camY == SHOWROOM_HEIGHT && isOverShowroom);

        if (!isCurrentlyInside && willBeInside && !landedOnRoof) {
            bool enteringViaGate = (camZ >= maxz) && (nextX > GATE_MIN_X && nextX < GATE_MAX_X);
            if (!enteringViaGate) { glutPostRedisplay(); return; }
        }
        else if (isCurrentlyInside && !willBeInside) {
            bool exitingViaGate = (camZ >= maxz - COLLISION_BUFFER) && (nextX > GATE_MIN_X && nextX < GATE_MAX_X);
            if (!exitingViaGate) { glutPostRedisplay(); return; }
        }

        // -- (هذا هو الإصلاح الجوهري) --
        // لا تتحقق من اصطدام الأجسام الداخلية إلا إذا كنت تحت السقف
        if (willBeInside && !landedOnRoof && camY <= SHOWROOM_HEIGHT) {
            BoundingBox playerBox = { nextX - 1.5f, camY - PLAYER_BUFFER_Y, nextZ - 1.5f, nextX + 1.5f, camY, nextZ + 1.5f };
            for (const auto& objBox : collidableObjects) {
                if (playerBox.maxX > objBox.minX && playerBox.minX < objBox.maxX && playerBox.maxY > objBox.minY && playerBox.minY < objBox.maxY && playerBox.maxZ > objBox.minZ && playerBox.minZ < objBox.maxZ) {
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

    if (weatherstatus==2)
    myRain.draw();
    abrarCode.drawCars();

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

    for (int i = 1; i <= 3; i++) {
        myJeep.drawJeep(x_jeep, y_jeep, z_jeep * i, size_jeep);
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
    
    lake.setAlpha(0.3);
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