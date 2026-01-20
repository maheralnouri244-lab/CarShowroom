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
#include <iostream>
#include "GlassManager.h"

#define M_PI acos(-1)
#include "CameraSphere.h"
#include "include\stb_image.h"

using namespace std;


#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

unsigned int loadTextureFromFile(const char* path) {
    int w, h, n;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &w, &h, &n, 4);

    if (!data) return 0;

    unsigned int newTexID;
    glGenTextures(1, &newTexID);
    glBindTexture(GL_TEXTURE_2D, newTexID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return newTexID;
}

void drawSkyDome(float radius, int slices, int stacks, GLuint textureID) {
    glPushMatrix();

    // 1. إعدادات "الخامة" للسماء
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 2. إعدادات الإضاءة والعمق (مهم جداً!)
    glDisable(GL_LIGHTING);      // السماء لا تتأثر بضوء الشمس، هي المصدر!
    glDepthMask(GL_FALSE);       // نمنع السماء من كتابة العمق (تظل خلف كل شيء)

    // لون أبيض نقي لضمان ظهور التيكستشر بألوانه الأصلية
    glColor3f(1.0f, 1.0f, 1.0f);

    // 3. خوارزمية النحت (Spherical Draw Loop)
    // نتحرك لنصف التكرارات فقط (stacks / 2) لأننا نريد نصف كرة
    for (int j = 0; j < stacks / 2; j++) {

        // حساب زوايا الارتفاع (Latitude)
        double lat1 = (M_PI * j) / stacks;
        double lat2 = (M_PI * (j + 1)) / stacks;
        double sinLat1 = sin(lat1);
        double cosLat1 = cos(lat1);
        double sinLat2 = sin(lat2);
        double cosLat2 = cos(lat2);

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= slices; i++) {
            // حساب زوايا الدوران (Longitude)
            double lon = (2 * M_PI * i) / slices;
            double sinLon = sin(lon);
            double cosLon = cos(lon);

            // حساب إحداثيات النسيج (Texture Coordinates)
            // U: التكرار الأفقي، V: التكرار العمودي
            float u = (float)i / slices;
            float v1 = 1.0f - (2.0f * (float)j / stacks);       // تعديل لتناسب النصف
            float v2 = 1.0f - (2.0f * (float)(j + 1) / stacks);

            // النقطة الأولى (الشريط العلوي)
            glNormal3f(sinLon * sinLat1, cosLat1, cosLon * sinLat1);
            glTexCoord2f(u, v1);
            glVertex3f(radius * sinLon * sinLat1, radius * cosLat1, radius * cosLon * sinLat1);

            // النقطة الثانية (الشريط السفلي)
            glNormal3f(sinLon * sinLat2, cosLat2, cosLon * sinLat2);
            glTexCoord2f(u, v2);
            glVertex3f(radius * sinLon * sinLat2, radius * cosLat2, radius * cosLon * sinLat2);
        }
        glEnd();
    }

    // 4. استعادة الحالة الأصلية
    glDepthMask(GL_TRUE);        // إعادة تفعيل كتابة العمق
    glEnable(GL_LIGHTING);       // إعادة تفعيل الإضاءة
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// ==========================================
// (Lighting Control)
// ==========================================
bool isLightOn = false;

struct Camera {
    float posX, posY, posZ;   
    float lookX, lookY, lookZ;
};

float lookX = -1.0f, lookY = -1.0f, lookZ = -1.0f;


/*
Camera cameras[4] = {
    { -150.197f, 52.0f, 200.503f,lookX, lookY, lookZ },
    {  152.003f, 62.0f, 201.193f,lookX, lookY, lookZ },
    { -143.372f, 58.0f, 193.254f,lookX, lookY, lookZ },
    {  142.205f, 58.0f,-193.402f,lookX, lookY, lookZ }
};
*/

// Camera Variables
float camX = 500, camY = 300, camZ = 500.0f;
float yaw = -90.0f, pitch = 0.0f;
int lastMouseX, lastMouseY, weatherstatus = 0, currentCam=0;
const int CloudCount = 20;
bool firstMouse = true, ignoreWarp;

float maxX = 150, maxz = 200, diff = 45;
float minX = -maxX, minz = -maxz;
int centerX ;
int centerY , lastCam=0;

float lakeMinX = minX - 170.0f;
float lakeMaxX = minX + 170.0f;
float lakeMinZ = maxz + 100.0f;
float lakeMaxZ = maxz + 600.0f;

// Instances
Maherheader maher;
AbrarCode abrarCode;
Jeep_Builder_Final myJeep;
SaraCode saraCode;
GlassManager glassMgr;
GlassWindow lake = GlassWindow(minX - 170, -0.2, maxz + 100, minX - 170, -0.2, maxz + 600, minX + 140, -0.2, maxz + 600, minX + 170, -0.2, maxz + 100);
CameraSphere cam;


Camera cameras[4] = {
    { minX-0.5, 61, maxz+0.5,   0.833441, - 0.387516 ,0.393964 },
    {  maxX + 0.5, 61, maxz + 0.5, -0.843008 ,- 0.305695 ,0.442593 },
    { minX + 0.5, 59, maxz - 0.5,   0.798929, - 0.414692 ,- 0.435595 },
    {  maxX - 0.5, 59, minz + 0.5, -0.783688, - 0.393941 ,0.480253},
};


GLuint cctvTexIDs[4] = { 0, 0, 0, 0 }, dayskyTex, nightskyTex, groundTex;// lakeTex;
bool hasSnapshot[4] = { false, false, false, false },captureThisFrame;

const float SHOWROOM_HEIGHT = 60.0f;
const float GATE_WIDTH = 45.0f;
const float GATE_MIN_X = -GATE_WIDTH / 2.0f;
const float GATE_MAX_X = GATE_WIDTH / 2.0f;
const float PLAYER_BUFFER_Y = 5.0f;
const float COLLISION_BUFFER = 3.0f;
const float GLOBAL_GROUND_LEVEL = 2.0f;
float lastlookx = lookX, lastlooky = lookY, lastlookz = lookZ;
float freeCamYaw = -90.0f, freeCamPitch = 0.0f;

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
    if (currentCam){
        cameras[currentCam - 1].lookX = lookX;
        cameras[currentCam - 1].lookY = lookY;
        cameras[currentCam - 1].lookZ = lookZ;
}
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

    dayskyTex = loadTextureFromFile("images\\finalday2.png");
    nightskyTex = loadTextureFromFile("images\\finalnight2.png");
    //lakeTex = loadTextureFromFile("images\\output.png");

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    myJeep.init();

    glassMgr.initShowroomWindows();
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

void switchCamera(int newCam) {
    
    // minX+30, h+16, maxz-23
    if (sqrt(pow(camX-(minX + 30),2)+pow(camY-(60+16),2)+pow(camZ-(maxz - 23),2)) > 100)
        return;

    if (currentCam >= 1 && currentCam <= 4 && newCam != currentCam) {
        captureThisFrame = true;
        lastCam = currentCam;
    }
        if (currentCam == 0) {
            freeCamYaw = yaw;
            freeCamPitch = pitch;
        }

    currentCam = newCam;
    glutPostRedisplay();
}

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
        case 'g': case 'f': case 'n': case 'N': case 'm': case 'M':
            abrarCode.handleInput(key, camX, camY, camZ);
            break;
        case 'r':   weatherstatus = (weatherstatus + 1) % 3; break;
        case 'i':  cout << camX << " " << camY << " " << camZ << " " << lookX << " " << lookY << " " << lookZ << "\n"; break;
        case '1': switchCamera(0); break;
        case '2': switchCamera(1); break;
        case '3': switchCamera(2); break;
        case '4': switchCamera(3); break;
        case '5': switchCamera(4); break;
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
        abrarCode.handleInput(key, camX, camY, camZ);
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
   centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
   centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

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

void setCamera(int camIndex) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (camIndex == 0) {
        gluLookAt(camX, camY, camZ,
            camX + lookX, camY + lookY, camZ + lookZ,
            0.0f, 1.0f, 0.0f);
    }
    else {
        Camera& c = cameras[camIndex - 1];
        lookX = c.lookX;
        lookY = c.lookY;
        lookZ = c.lookZ;

        yaw = atan2(lookZ, lookX) * 180.0f / M_PI;
        pitch = asin(lookY) * 180.0f / M_PI;

        gluLookAt(c.posX, c.posY, c.posZ,
            c.posX + lookX, c.posY + lookY, c.posZ + lookZ,
            0.0f, 1.0f, 0.0f);
    }
}

void drawscene(bool drawcam=1)
{
    drawSkyBody(isLightOn);
    GLuint skyTex;


    if (isLightOn) {
        glEnable(GL_LIGHT0);
        GLfloat ambientDay[] = { 0.4f, 0.4f, 0.4f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientDay);
        skyTex = dayskyTex;
    }
    else {
        glDisable(GL_LIGHT0);
        GLfloat ambientNight[] = { 0.3f, 0.3f, 0.5f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientNight);
        skyTex = nightskyTex;
    }

    if (weatherstatus == 2)
        myRain.draw();
    abrarCode.drawCars();
    abrarCode.drawGroundFloorElevator();
    abrarCode.drawSecondFloor();
    saraCode.drawAll();
    maher.draw(camX, camY, camZ, isLightOn, cctvTexIDs);
    //glassMgr.drawAll();
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

    /*//glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, lakeTex);

    glBegin(GL_QUADS);
    //glTexCoord2f(0.0f, 0.0f);
    glVertex3f(lakeMinX, 0.0f, lakeMinZ);

    //glTexCoord2f(1.0f, 0.0f);
    glVertex3f(lakeMaxX, 0.0f, lakeMinZ);

    //glTexCoord2f(1.0f, 1.0f);
    glVertex3f(lakeMaxX, 0.0f, lakeMaxZ);

    //glTexCoord2f(0.0f, 1.0f);
    glVertex3f(lakeMinX, 0.0f, lakeMaxZ);
    glEnd();

    //glDisable(GL_TEXTURE_2D);*/


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


    if (drawcam && currentCam != 1)
    {
        glPushMatrix();
        glTranslatef(minX, 60, maxz);
        glRotatef(270, 0, 1, 0);
        cam.setLookDirection(cameras[0].lookZ, cameras[0].lookY, -cameras[0].lookX);
        cam.draw();
        glPopMatrix();
    }

    if (drawcam && currentCam != 2)
    {
        glPushMatrix();
        glTranslatef(maxX, 60, maxz);
        cam.setLookDirection(cameras[1].lookX, cameras[1].lookY, cameras[1].lookZ);
        cam.draw();
        glPopMatrix();
    }

    if (drawcam && currentCam != 3)
    {
        glPushMatrix();
        glTranslatef(minX + 1, 60 - 1, maxz - 1);
        glRotatef(90, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        cam.setLookDirection(-cameras[2].lookZ, cameras[2].lookX, -cameras[2].lookY);

        cam.draw();
        glPopMatrix();
    }

    if (drawcam && currentCam != 4)
    {
        glPushMatrix();
        glTranslatef(maxX - 1, 60 - 1, minz + 1);
        glRotatef(270, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        cam.setLookDirection(cameras[3].lookZ, -cameras[3].lookX, -cameras[3].lookY);
        cam.draw();
        glPopMatrix();
    }

    // نفترض أن lakeTex هو GLuint للـ texture محمّل مسبقاً
    

    drawSkyDome(2000, 64, 64, skyTex);


    glPushMatrix();
    glTranslatef(0, -1, 0);
    glScalef(1, -1, 1);
    if (weatherstatus == 2)
        myRain.draw();
    maher.draw(camX, camY, camZ, isLightOn,cctvTexIDs);
    saraCode.drawAll();
    drawSkyBody(isLightOn);
    for (int i = 0; weatherstatus > 0 && i < CloudCount; i++) {
        myClouds[i].draw(weatherstatus);
    }
    drawSkyDome(2000, 64, 64, skyTex);


    glPushMatrix();
    glTranslatef(minX, 60, maxz);
    glRotatef(270, 0, 1, 0);
    cam.setLookDirection(cameras[0].lookZ, cameras[0].lookY, -cameras[0].lookX);
    cam.draw();
    glPopMatrix();
        glPushMatrix();
        glTranslatef(maxX, 60, maxz);
        cam.setLookDirection(cameras[1].lookX, cameras[1].lookY, cameras[1].lookZ);
        cam.draw();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(minX + 1, 60 - 1, maxz - 1);
        glRotatef(90, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        cam.setLookDirection(-cameras[2].lookZ, cameras[2].lookX, -cameras[2].lookY);

        cam.draw();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(maxX - 1, 60 - 1, minz + 1);
        glRotatef(270, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        cam.setLookDirection(cameras[3].lookZ, -cameras[3].lookX, -cameras[3].lookY);
        cam.draw();
        glPopMatrix();

    abrarCode.drawCars();
    abrarCode.drawGroundFloorElevator();
    abrarCode.drawSecondFloor();

    glPopMatrix();
    lake.setAlpha(0.7);
    lake.draw();
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


    if (captureThisFrame) {
        int idx = lastCam - 1;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setCamera(lastCam);
        drawscene(0);
        glassMgr.drawAll();
        if (cctvTexIDs[idx] == 0) glGenTextures(1, &cctvTexIDs[idx]);
        glBindTexture(GL_TEXTURE_2D, cctvTexIDs[idx]);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 1200, 800, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        hasSnapshot[idx] = true;
        captureThisFrame = false;

        if (currentCam == 0) {
            yaw = freeCamYaw;
            pitch = freeCamPitch;
            updateLookVector();
        }
    }

    setCamera(currentCam);
    drawscene();
    glassMgr.drawAll();

    glutSwapBuffers();
}

void handleResize(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 4000.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    srand(time(0));
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Showroom Simulator");

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