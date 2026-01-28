#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Pillar.h"
#include "Hpillar.h"
#include "Showroomside.h"
#include "ShowroomGate.h"
#include "GlassWindow.h"
#include "PlusRing3D.h"
#include "Maherheader.h"
#include "AbrarCode.h"
#include "JeepAsset.h"
#include "PersistentCloud.h"
#include "RainSystem.h"
#include "SaraCode.h"
#include "GlassManager.h"
#include "CameraSphere.h"
#include "CityMasterPlan.h"
#include "F1Car.h"
#include "Lake.h" 
#include "include\\stb_image.h"
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include "sound.h"
using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float camX = 500, camY = 300, camZ = 500.0f;
float yaw = -90.0f, pitch = 0.0f;
float lookX = -1.0f, lookY = -1.0f, lookZ = -1.0f;
int lastMouseX, lastMouseY, weatherstatus = 0, currentCam = 0;
bool firstMouse = true, ignoreWarp;
int centerX, centerY, lastCam = 0;

float maxX = 150, maxz = 200, diff = 45;
float minX = -maxX, minz = -maxz;

Maherheader maher;
AbrarCode abrarCode;
Jeep_Builder_Final myJeep;
SaraCode saraCode;
GlassManager glassMgr;
CameraSphere cam;
F1Car carRed(0.8f, 0.1f, 0.1f);
F1Car carBlue(0.1f, 0.3f, 0.8f);
F1Car carYellow(0.9f, 0.8f, 0.1f);

Lake myReflectionLake(-280.0f, 0.06f, 600.0f, 200.0f, 300.0f, 0.6f);

struct Camera {
    float posX, posY, posZ;
    float lookX, lookY, lookZ;
};

Camera cameras[4] = {
    { minX - 0.5f, 61, maxz + 0.5f,   0.833441, -0.387516 ,0.393964 },
    { maxX + 0.5f, 61, maxz + 0.5f, -0.843008 ,-0.305695 ,0.442593 },
    { minX + 0.5f, 59, maxz - 0.5f,   0.798929, -0.414692 ,-0.435595 },
    { maxX - 0.5f, 59, minz + 0.5f, -0.783688, -0.393941 ,0.480253},
};

GLuint cctvTexIDs[4] = { 0, 0, 0, 0 }, dayskyTex, nightskyTex, groundTex;
bool hasSnapshot[4] = { false, false, false, false }, captureThisFrame;

const float SHOWROOM_HEIGHT = 60.0f;
const float GATE_WIDTH = 45.0f;
const float GATE_MIN_X = -GATE_WIDTH / 2.0f;
const float GATE_MAX_X = GATE_WIDTH / 2.0f;
const float PLAYER_BUFFER_Y = 5.0f;
const float COLLISION_BUFFER = 3.0f;
const float GLOBAL_GROUND_LEVEL = 2.0f;
float lastlookx = lookX, lastlooky = lookY, lastlookz = lookZ;
float freeCamYaw = -90.0f, freeCamPitch = 0.0f;

std::vector<BoundingBox> collidableObjects;

const int CloudCount = 30;
PersistentCloud myClouds[CloudCount];
RainSystem myRain(3000);
bool isLightOn = true;

struct ParkingCar {
    float x, y, z, rotation;
    int seed;
};

std::vector<ParkingCar> parkingLotCars;

void updateAmbientSound();
void addCityCollisions(std::vector<BoundingBox>& collidables);


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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    glColor3f(1.0f, 1.0f, 1.0f);

    for (int j = 0; j < stacks / 2; j++) {
        double lat1 = (M_PI * j) / stacks;
        double lat2 = (M_PI * (j + 1)) / stacks;
        double sinLat1 = sin(lat1);
        double cosLat1 = cos(lat1);
        double sinLat2 = sin(lat2);
        double cosLat2 = cos(lat2);

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= slices; i++) {
            double lon = (2 * M_PI * i) / slices;
            double sinLon = sin(lon);
            double cosLon = cos(lon);

            float u = (float)i / slices;
            float v1 = 1.0f - (2.0f * (float)j / stacks);
            float v2 = 1.0f - (2.0f * (float)(j + 1) / stacks);

            glNormal3f(sinLon * sinLat1, cosLat1, cosLon * sinLat1);
            glTexCoord2f(u, v1);
            glVertex3f(radius * sinLon * sinLat1, radius * cosLat1, radius * cosLon * sinLat1);

            glNormal3f(sinLon * sinLat2, cosLat2, cosLon * sinLat2);
            glTexCoord2f(u, v2);
            glVertex3f(radius * sinLon * sinLat2, radius * cosLat2, radius * cosLon * sinLat2);
        }
        glEnd();
    }
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
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

bool isStrictlyInside(float x, float y, float z) {
    bool horizontalCheck = (x > minX && x < maxX && z > minz && z < maxz);
    bool verticalCheck = (y >= 0.0f && y < 125.0f);

    return horizontalCheck && verticalCheck;
}

void updateLookVector() {
    if (abrarCode.getState() == STATE_ENTERING) return;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    float radYaw = yaw * M_PI / 180.0f;
    float radPitch = pitch * M_PI / 180.0f;
    lookX = cos(radPitch) * cos(radYaw);
    lookY = sin(radPitch);
    lookZ = cos(radPitch) * sin(radYaw);
    if (currentCam) {
        cameras[currentCam - 1].lookX = lookX;
        cameras[currentCam - 1].lookY = lookY;
        cameras[currentCam - 1].lookZ = lookZ;
    }
}

void switchCamera(int newCam) {
    if (sqrt(pow(camX - (minX + 30), 2) + pow(camY - (60 + 16), 2) + pow(camZ - (maxz - 23), 2)) > 100)
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

void initParkingLotCars() {
    parkingLotCars.clear();

    float pX = 350.0f;
    float pZ = 600.0f;
    float pW = 300.0f;
    float pL = 400.0f;
    int numRows = 3;
    float rowHeight = pL / numRows;

    for (int i = 0; i < numRows; i++) {
        float cZ = (pZ - pL / 2) + rowHeight * i + rowHeight / 2;
        int spots = 7;
        float sW = (pW - 40) / spots;

        for (int k = 0; k < spots; k++) {
            int randSeed = (int)(i * 99 + k * 17);

            if (randSeed % 3 != 0) {
                float carX = (pX - (pW - 40) / 2) + k * sW + sW / 2;
                float carZ_pos = cZ - (rowHeight - 20) / 2 + 15.0f;
                parkingLotCars.push_back({ carX, 0.15f, carZ_pos, 90.0f, randSeed });
            }

            if ((randSeed + 1) % 3 != 0) {
                float carX = (pX - (pW - 40) / 2) + k * sW + sW / 2;
                float carZ_pos = cZ + (rowHeight - 20) / 2 - 15.0f;
                parkingLotCars.push_back({ carX, 0.15f, carZ_pos, -90.0f, randSeed + 7 });
            }
        }
    }
}

void addCityCollisions(std::vector<BoundingBox>& collidables) {

#pragma region Showroom Plaza (الساحة أمام المعرض)
    collidables.push_back({ 165.0f, 0.0f, -225.0f, 175.0f, 15.0f, -215.0f });
    collidables.push_back({ -165.0f, 0.0f, -225.0f, -155.0f, 15.0f, -215.0f });

    collidables.push_back({ 55.0f, 0.0f, 235.0f, 65.0f, 2.0f, 245.0f });
    collidables.push_back({ -65.0f, 0.0f, 235.0f, -55.0f, 2.0f, 245.0f });

    collidables.push_back({ -70.0f, 100.0f, 201.0f, 70.0f, 110.0f, 205.0f });
#pragma endregion

#pragma region Main Boulevard (الشارع الرئيسي)
    float startZ_boulevard = 280.0f;
    float entryLength_boulevard = 100.0f;
    float splitZ_boulevard = startZ_boulevard + entryLength_boulevard;
    float roundaboutCenterZ = 600.0f;
    float northStart = roundaboutCenterZ + 120.0f;
    float medianWidth = 20.0f;
    float laneWidth = 40.0f;
    float lampX_boulevard = medianWidth / 2 + laneWidth + 10;


    for (int i = 0; i < 3; i++) {
        float lampZ = splitZ_boulevard + 20 + i * 60.0f;
        collidables.push_back({ lampX_boulevard - 1.0f, 0.0f, lampZ - 1.0f, lampX_boulevard + 1.0f, 22.0f, lampZ + 1.0f });
        collidables.push_back({ -lampX_boulevard - 1.0f, 0.0f, lampZ - 1.0f, -lampX_boulevard + 1.0f, 22.0f, lampZ + 1.0f });
    }
    for (int i = 0; i < 18; i++) {
        float lampZ = northStart + 40 + i * 80.0f;
        collidables.push_back({ lampX_boulevard - 1.0f, 0.0f, lampZ - 1.0f, lampX_boulevard + 1.0f, 22.0f, lampZ + 1.0f });
        collidables.push_back({ -lampX_boulevard - 1.0f, 0.0f, lampZ - 1.0f, -lampX_boulevard + 1.0f, 22.0f, lampZ + 1.0f });
    }
#pragma endregion

#pragma region Roundabout (الدوار)
    collidables.push_back({ -10.0f, 0.0f, 590.0f, 10.0f, 20.0f, 610.0f });
    collidables.push_back({ 10.0f, 0.0f, 595.0f, 20.0f, 2.0f, 605.0f });
    collidables.push_back({ -20.0f, 0.0f, 595.0f, -10.0f, 2.0f, 605.0f });
    collidables.push_back({ -5.0f, 0.0f, 610.0f, 5.0f, 2.0f, 620.0f });
    collidables.push_back({ -5.0f, 0.0f, 580.0f, 5.0f, 2.0f, 590.0f });
#pragma endregion

#pragma region Lake Park (حديقة البحيرة)
    float parkX = -280.0f, parkZ = 600.0f;
    float parkW = 350.0f, parkL = 500.0f;
    float lakeW = 200.0f, lakeL = 300.0f;

    collidables.push_back({ (parkX + lakeW / 2 + 40) - 15, 0.0f, (parkZ - lakeL / 2 + 40) - 15, (parkX + lakeW / 2 + 40) + 15, 20.0f, (parkZ - lakeL / 2 + 40) + 15 });
    collidables.push_back({ (parkX - parkW / 2 + 40) - 6, 0.0f, (parkZ - parkL / 2 + 40) - 6, (parkX - parkW / 2 + 40) + 6, 10.0f, (parkZ - parkL / 2 + 40) + 6 });
    float benchStart = parkZ - 120;
    for (int i = 0; i < 5; i++) {
        float posZ = benchStart + i * 60;
        collidables.push_back({ (parkX + lakeW / 2 + 20) - 3, 0.0f, posZ - 7, (parkX + lakeW / 2 + 20) + 3, 8.0f, posZ + 7 });
        collidables.push_back({ (parkX + lakeW / 2 + 35) - 1, 0.0f, posZ - 1, (parkX + lakeW / 2 + 35) + 1, 22.0f, posZ + 1 });
        collidables.push_back({ (parkX - lakeW / 2 - 35) - 1, 0.0f, posZ - 1, (parkX - lakeW / 2 - 35) + 1, 22.0f, posZ + 1 });
        collidables.push_back({ (parkX - lakeW / 2 - 20) - 3, 0.0f, posZ - 7, (parkX - lakeW / 2 - 20) + 3, 8.0f, posZ + 7 });
    }

    collidables.push_back({ (parkX + parkW / 2) - 50, 0.0f, parkZ - 5, (parkX + parkW / 2) + 50, 25.0f, parkZ + 5 }); // البوابة
    float poleH = 8.0f, gap = 15.0f;
    for (float i = -parkW / 2; i <= parkW / 2; i += gap) {
        collidables.push_back({ parkX + i - 0.5f, 0.0f, parkZ - parkL / 2 - 0.5f, parkX + i + 0.5f, poleH, parkZ - parkL / 2 + 0.5f });
        if (abs(i) > 40) {
            collidables.push_back({ parkX + i - 0.5f, 0.0f, parkZ + parkL / 2 - 0.5f, parkX + i + 0.5f, poleH, parkZ + parkL / 2 + 0.5f });
        }
    }
    for (float i = -parkL / 2; i <= parkL / 2; i += gap) {
        collidables.push_back({ parkX + parkW / 2 - 0.5f, 0.0f, parkZ + i - 0.5f, parkX + parkW / 2 + 0.5f, poleH, parkZ + i + 0.5f });
        collidables.push_back({ parkX - parkW / 2 - 0.5f, 0.0f, parkZ + i - 0.5f, parkX - parkW / 2 + 0.5f, poleH, parkZ + i + 0.5f });
    }
#pragma endregion

    float carBoxWidth = 10.0f;
    float carBoxLength = 22.0f;
    float carBoxHeight = 12.0f;

    for (const auto& car : parkingLotCars) {
        collidableObjects.push_back({
            car.x - carBoxLength / 2, car.y, car.z - carBoxWidth / 2,
            car.x + carBoxLength / 2, car.y + carBoxHeight, car.z + carBoxWidth / 2
            });
    }
#pragma region Parking Zone (منطقة المواقف)
    float pX = 350.0f, pZ = 600.0f, pW = 300.0f, pL = 400.0f;
    float perimX = pW / 2 + 20.0f, perimZ = pL / 2 + 20.0f;

    for (float z_coord = -perimZ; z_coord <= perimZ; z_coord += 60) {


        collidables.push_back({ pX + perimX - 1, 0.0f, pZ + z_coord + 30 - 1, pX + perimX + 1, 22.0f, pZ + z_coord + 30 + 1 });


    }
    for (float x_coord = -perimX; x_coord <= perimX; x_coord += 60) {
        collidables.push_back({ pX + x_coord - 5, 0.0f, pZ - perimZ - 5, pX + x_coord + 5, 15.0f, pZ - perimZ + 5 });
        collidables.push_back({ pX + x_coord - 5, 0.0f, pZ + perimZ - 5, pX + x_coord + 5, 15.0f, pZ + perimZ + 5 });
    }

    int numRows_park = 3; float rowHeight_park = pL / numRows_park;
    for (int i = 0; i < numRows_park; i++) {
        float cZ = (pZ - pL / 2) + rowHeight_park * i + rowHeight_park / 2;
        if (i < numRows_park - 1) {
            collidables.push_back({ pX - (pW - 20) / 2, 0.0f, cZ + rowHeight_park / 2 - 4, pX + (pW - 20) / 2, 1.0f, cZ + rowHeight_park / 2 + 4 });
            collidables.push_back({ pX - 1, 0.0f, cZ + rowHeight_park / 2 - 1, pX + 1, 22.0f, cZ + rowHeight_park / 2 + 1 });
        }
        collidables.push_back({ pX - (pW - 40) / 2, 0.0f, cZ - (rowHeight_park - 20) / 2 + 1, pX + (pW - 40) / 2, 15.0f, cZ - (rowHeight_park - 20) / 2 + 3 });
    }
#pragma endregion

#pragma region Commercial Zone (المنطقة التجارية)
    float cX = -350.0f, cZ = 200.0f;
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            float tx = cX - 150.0f + col * 300.0f;
            float tz = cZ - 100.0f + row * 200.0f;
            collidables.push_back({ tx - 15, 0.0f, tz - 15, tx + 15, 15.0f, tz + 15 });
        }
    }
    collidables.push_back({ (cX - 75) - 8, 0.0f, (cZ + 100) - 6, (cX - 75) + 8, 13.0f, (cZ + 100) + 6 });
    collidables.push_back({ (cX + 75) - 8, 0.0f, (cZ + 100) - 6, (cX + 75) + 8, 13.0f, (cZ + 100) + 6 });
    collidables.push_back({ cX - 5, 0.0f, cZ - 5, cX + 5, 2.0f, cZ + 5 });
#pragma endregion
    float trees[21][3] = {
        { 165.0f, 0.0f, 215.0f },
        { -165.0f, 0.0f, 215.0f },
        { 0.0f, 0.0f, 770.0f },
        { 0.0f, 0.0f, 970.0f },
        { 0.0f, 0.0f, 1170.0f },
        { 0.0f, 0.0f, 1370.0f },
        { 0.0f, 0.0f, 1570.0f },
        { 0.0f, 0.0f, 1770.0f },
        { 0.0f, 0.0f, 1970.0f },
        { 520.0f, 0.0f, 500.0f },
        { 520.0f, 0.0f, 600.0f },
        { 520.0f, 0.0f, 700.0f },
        { 180.0f, 0.0f, 500.0f },
        { 180.0f, 0.0f, 600.0f },
        { 180.0f, 0.0f, 700.0f },
        { 350.0f, 0.0f, 820.0f },
        { -425.0f, 0.0f, 450.0f },
        { -425.0f, 0.0f, 750.0f },
        { -135.0f, 0.0f, 450.0f },
        { -135.0f, 0.0f, 750.0f },
        { -280.0f, 0.0f, 380.0f }
    };
    for (int i = 0; i < 21; i++) {
        float tx = trees[i][0];
        float tz = trees[i][2];

        collidables.push_back({
            tx - 6.0f, -10.0f, tz - 6.0f,
            tx + 6.0f,  40.0f, tz + 6.0f 
            });
    }
}

void addAbrarCodeCollisions(std::vector<BoundingBox>& collidables) {
    const float FLOOR_H = 60.0f;

    float steveWidth = 2.0f;
    float steveDepth = 2.0f;
    float steveHeight = 5.0f;

#pragma region AbrarCode Second Floor Steves

    float secretaryX = 120.0f + 15.0f;
    float secretaryZ = 50.0f - 30.0f;
    collidables.push_back({ secretaryX - steveWidth, FLOOR_H, secretaryZ - steveDepth, secretaryX + steveWidth, FLOOR_H + steveHeight, secretaryZ + steveDepth });
    float cashierX = -100.0f + 50.0f;
    float cashierZ = 0.0f - 180.0f - 6.0f;
    collidables.push_back({ cashierX - steveWidth, FLOOR_H, cashierZ - steveDepth, cashierX + steveWidth, FLOOR_H + steveHeight, cashierZ + steveDepth });

    float queueBaseX = -100.0f + 50.0f;
    float queueBaseZ = -155.0f;
    for (int i = 0; i < 6; i++) {
        float personZ = queueBaseZ + 20 + i * 15;
        collidables.push_back({ queueBaseX - steveWidth, FLOOR_H, personZ - steveDepth, queueBaseX + steveWidth, FLOOR_H + steveHeight, personZ + steveDepth });
    }

    float peopleCoords[10][3] = {
        {80, 0.2f, -50},
        {-80, 0.2f, 50},
        {0, 0.2f, 100},
        {50, 0.2f, 150},
        {-50, 0.2f, -100},
        {100, 0.2f, 0},
        {-100, 0.2f, 0},
        {20, 0.2f, 20},
        {-20, 0.2f, -20},
        {0, 0.2f, -150}
    };
    for (int i = 0; i < 10; i++) {
        float personX = peopleCoords[i][0];
        float personZ = peopleCoords[i][2];
        collidables.push_back({ personX - steveWidth, FLOOR_H, personZ - steveDepth, personX + steveWidth, FLOOR_H + steveHeight, personZ + steveDepth });
    }
#pragma endregion
}


void createCollidables() {
    collidableObjects.clear();
    float wallBuffer = 5.0f;
    float floorThickness = 2.0f;

    collidableObjects.push_back({ minX, 0.0f, maxz - wallBuffer, GATE_MIN_X, SHOWROOM_HEIGHT, maxz + wallBuffer });
    collidableObjects.push_back({ GATE_MAX_X, 0.0f, maxz - wallBuffer, maxX, SHOWROOM_HEIGHT, maxz + wallBuffer });

    float wall_1_x = minX + (maxX - minX - diff) / 2.0f;
    float wall_2_x = maxX - (maxX - minX - diff) / 2.0f;
    float wall_z_range = (maxz - minz - 2 * diff) / 2.0f;
    collidableObjects.push_back({ wall_1_x - wallBuffer, 0.0f, -wall_z_range, wall_1_x + wallBuffer, SHOWROOM_HEIGHT, wall_z_range });
    collidableObjects.push_back({ wall_2_x - wallBuffer, 0.0f, -wall_z_range, wall_2_x + wallBuffer, SHOWROOM_HEIGHT, wall_z_range });
    collidableObjects.push_back({ minX, 0.0f, minz - wallBuffer, maxX, SHOWROOM_HEIGHT, minz + wallBuffer });
    float extentX = (40.0f / 2.0f) * 3.0f;
    float extentZ = (40.0f / 2.0f) * 1.0f;
    float center_x_C1 = -minX - (maxX - minX - diff) / 4.0f;
    float center_x_C2 = minX + (maxX - minX - diff) / 4.0f;
    collidableObjects.push_back({ center_x_C1 - extentX, 0.0f, -extentZ, maxX, 30.0f, extentZ });
    collidableObjects.push_back({ minX, 0.0f, -extentZ, center_x_C2 + extentX, 30.0f, extentZ });
    float beetle_x = 120.0f;
    float beetle_z[] = { -40.0f, -75.0f, -110.0f, -145.0f };
    for (float z : beetle_z) {
        collidableObjects.push_back({ beetle_x - 18.0f, 0.0f, z - 10.0f, beetle_x + 18.0f, 15.0f, z + 10.0f });
    }

    float myCarX = -110.0f;
    float myCarZ[] = { 160.0f, 120.0f, 80.0f, 40.0f };
    float carBoxWidth = 10.0f;
    float carBoxLength = 10.0f;
    float sideSafety = 3.0f;
    for (float zPos : myCarZ) {
        collidableObjects.push_back({ myCarX - (carBoxWidth + sideSafety), 0.0f, zPos - carBoxLength, myCarX + (carBoxWidth + sideSafety), 20.0f, zPos + carBoxLength });
    }
    float jeep_x = 120.0f;
    float jeep_safety_X = 5.0f;
    float jeep_box_Z = 12.0f;
    for (int i = 1; i <= 3; i++) {
        float jZ = 50.0f * i;
        collidableObjects.push_back({ jeep_x - (15.0f + jeep_safety_X), 0.0f, jZ - jeep_box_Z, jeep_x + (15.0f + jeep_safety_X), 25.0f, jZ + jeep_box_Z });
    }

    const float SECOND_FLOOR_Y_START = SHOWROOM_HEIGHT;
    const float SECOND_FLOOR_HEIGHT = 60.0f;
    const float SECOND_FLOOR_Y_END = SECOND_FLOOR_Y_START + SECOND_FLOOR_HEIGHT;
    const float ELEV_X = -120.0f, ELEV_Z = -170.0f, ELEV_WIDTH = 30.0f;
    const float ELEV_MIN_X = ELEV_X - ELEV_WIDTH / 2.0f, ELEV_MAX_X = ELEV_X + ELEV_WIDTH / 2.0f;
    const float ELEV_MIN_Z = ELEV_Z - ELEV_WIDTH / 2.0f, ELEV_MAX_Z = ELEV_Z + ELEV_WIDTH / 2.0f;
    collidableObjects.push_back({ minX, SECOND_FLOOR_Y_START, minz - wallBuffer, maxX, SECOND_FLOOR_Y_END, minz + wallBuffer });
    collidableObjects.push_back({ minX, SECOND_FLOOR_Y_START, maxz - wallBuffer, maxX, SECOND_FLOOR_Y_END, maxz + wallBuffer });
    collidableObjects.push_back({ minX - wallBuffer, SECOND_FLOOR_Y_START, minz, minX + wallBuffer, SECOND_FLOOR_Y_END, maxz });
    collidableObjects.push_back({ maxX - wallBuffer, SECOND_FLOOR_Y_START, minz, maxX + wallBuffer, SECOND_FLOOR_Y_END, maxz });
    collidableObjects.push_back({ minX, SECOND_FLOOR_Y_END - floorThickness, minz, maxX, SECOND_FLOOR_Y_END, maxz });
    collidableObjects.push_back({ minX, SECOND_FLOOR_Y_START - floorThickness, ELEV_MAX_Z, maxX, SECOND_FLOOR_Y_START, maxz });
    collidableObjects.push_back({ minX, SECOND_FLOOR_Y_START - floorThickness, minz, maxX, SECOND_FLOOR_Y_START, ELEV_MIN_Z });
    collidableObjects.push_back({ minX, SECOND_FLOOR_Y_START - floorThickness, ELEV_MIN_Z, ELEV_MIN_X, SECOND_FLOOR_Y_START, ELEV_MAX_Z });
    collidableObjects.push_back({ ELEV_MAX_X, SECOND_FLOOR_Y_START - floorThickness, ELEV_MIN_Z, maxX, SECOND_FLOOR_Y_START, ELEV_MAX_Z });
    const float FLOOR_H = 60.0f;

    collidableObjects.push_back({ 30.0f, FLOOR_H, -184.0f, 70.0f, FLOOR_H + 10.0f, -176.0f });

    collidableObjects.push_back({ 90.0f, FLOOR_H, 0.0f, 150.0f, FLOOR_H + 40.0f, 100.0f });

    collidableObjects.push_back({ 90.0f, FLOOR_H, 110.0f, 110.0f, FLOOR_H + 10.0f, 130.0f });

    collidableObjects.push_back({ 112.0f, FLOOR_H, 142.0f, 128.0f, FLOOR_H + 5.0f, 158.0f });

    collidableObjects.push_back({ 92.5f, FLOOR_H, 142.5f, 107.5f, FLOOR_H + 2.0f, 157.5f });

    float shelfCenterX = -120.0f;
    float shelfWidth = 36.0f;
    float shelfDepth = 4.0f;
    float shelfHeight = 30.0f;
    float shelfSpacing = 50.0f;
    float startZ = -100.0f;

    for (int i = 0; i < 5; i++) {
        float currentCenterZ = startZ + (i * shelfSpacing);

        float min_x = shelfCenterX - shelfWidth / 2.0f;
        float max_x = shelfCenterX + shelfWidth / 2.0f;
        float min_y = FLOOR_H;
        float max_y = FLOOR_H + shelfHeight;
        float min_z = currentCenterZ - shelfDepth / 2.0f;
        float max_z = currentCenterZ + shelfDepth / 2.0f;

        collidableObjects.push_back({ min_x, min_y, min_z, max_x, max_y, max_z });
    }

    collidableObjects.push_back({ -125.0f, 0.0f, -55.0f, -115.0f, 10.0f, -45.0f });

    collidableObjects.push_back({ -125.0f, 0.0f, -95.0f, -115.0f, 10.0f, -85.0f });

    collidableObjects.push_back({ -125.0f, 0.0f, -135.0f, -115.0f, 10.0f, -125.0f });

    float pX = 350.0f;
    float pZ = 600.0f;
    float pW = 300.0f;
    float pL = 400.0f;
    int numRows = 3;
    float rowHeight = pL / numRows;

    float carBoxHeight = 7.0f;

    for (int i = 0; i < numRows; i++) {
        float cZ = (pZ - pL / 2) + rowHeight * i + rowHeight / 2;
        int spots = 7;
        float sW = (pW - 40) / spots;

        for (int k = 0; k < spots; k++) {
            int randSeed = (int)(i * 99 + k * 17);

            if (randSeed % 3 != 0) {
                float carX = (pX - (pW - 40) / 2) + k * sW + sW / 2;
                float carZ = cZ - (rowHeight - 20) / 2 + 15.0f;
                collidableObjects.push_back({
                    carX - carBoxWidth / 2, 0.15f, carZ - carBoxLength / 2,
                    carX + carBoxWidth / 2, 0.15f + carBoxHeight, carZ + carBoxLength / 2
                    });
            }

            if ((randSeed + 1) % 3 != 0) {
                float carX = (pX - (pW - 40) / 2) + k * sW + sW / 2;
                float carZ = cZ + (rowHeight - 20) / 2 - 15.0f;
                collidableObjects.push_back({
                    carX - carBoxWidth / 2, 0.15f, carZ - carBoxLength / 2,
                    carX + carBoxWidth / 2, 0.15f + carBoxHeight, carZ + carBoxLength / 2
                    });
            }
        }
    }
    float steveData[41][15] = {
        { -40.0f, 230.0f, 180.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.00f, 0.65f, 0.65f, 0.25f, 0.25f, 0.60f },
        { 40.0f, 250.0f, 170.0f, 0.80f, 0.55f, 0.40f, 0.20f, 0.20f, 0.20f, 0.80f, 0.20f, 0.20f, 0.10f, 0.10f, 0.40f },
        { 60.0f, 280.0f, 190.0f, 0.60f, 0.40f, 0.30f, 0.10f, 0.10f, 0.10f, 0.20f, 0.70f, 0.20f, 0.30f, 0.30f, 0.30f },
        { -80.0f, 300.0f, 160.0f, 0.75f, 0.50f, 0.38f, 0.30f, 0.20f, 0.10f, 0.90f, 0.90f, 0.20f, 0.20f, 0.20f, 0.50f },
        { 100.0f, 240.0f, 200.0f, 0.50f, 0.35f, 0.25f, 0.10f, 0.05f, 0.05f, 0.50f, 0.50f, 0.50f, 0.15f, 0.15f, 0.20f },
        { -120.0f, 260.0f, 180.0f, 0.70f, 0.47f, 0.35f, 0.40f, 0.30f, 0.20f, 0.20f, 0.20f, 0.80f, 0.10f, 0.10f, 0.10f },
        { 140.0f, 350.0f, 175.0f, 0.85f, 0.60f, 0.45f, 0.60f, 0.50f, 0.30f, 0.70f, 0.40f, 0.20f, 0.30f, 0.30f, 0.50f },
        { 20.0f, -230.0f, 0.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.40f, 0.10f, 0.60f, 0.20f, 0.20f, 0.20f },
        { -50.0f, -260.0f, 10.0f, 0.65f, 0.45f, 0.30f, 0.15f, 0.15f, 0.15f, 0.20f, 0.60f, 0.80f, 0.25f, 0.25f, 0.40f },
        { 90.0f, -290.0f, 350.0f, 0.55f, 0.38f, 0.28f, 0.05f, 0.05f, 0.05f, 0.80f, 0.80f, 0.80f, 0.20f, 0.20f, 0.30f },
        { -10.0f, -320.0f, 5.0f, 0.72f, 0.48f, 0.36f, 0.35f, 0.25f, 0.15f, 0.10f, 0.50f, 0.10f, 0.40f, 0.40f, 0.30f },
        { -130.0f, -240.0f, 340.0f, 0.80f, 0.55f, 0.40f, 0.50f, 0.40f, 0.20f, 0.90f, 0.50f, 0.10f, 0.20f, 0.20f, 0.50f },
        { 110.0f, -270.0f, 15.0f, 0.60f, 0.40f, 0.30f, 0.20f, 0.10f, 0.05f, 0.30f, 0.30f, 0.60f, 0.15f, 0.15f, 0.25f },
        { -90.0f, -380.0f, 0.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.70f, 0.20f, 0.40f, 0.25f, 0.25f, 0.60f },
        { 180.0f, 0.0f, 270.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.00f, 0.65f, 0.65f, 0.25f, 0.25f, 0.60f },
        { 200.0f, 80.0f, 260.0f, 0.50f, 0.35f, 0.25f, 0.10f, 0.10f, 0.10f, 0.50f, 0.10f, 0.10f, 0.10f, 0.10f, 0.10f },
        { 220.0f, -60.0f, 280.0f, 0.80f, 0.55f, 0.40f, 0.40f, 0.30f, 0.10f, 0.20f, 0.60f, 0.20f, 0.20f, 0.30f, 0.20f },
        { 250.0f, 120.0f, 275.0f, 0.70f, 0.47f, 0.35f, 0.30f, 0.20f, 0.15f, 0.10f, 0.40f, 0.70f, 0.30f, 0.30f, 0.40f },
        { 190.0f, -150.0f, 265.0f, 0.65f, 0.45f, 0.30f, 0.20f, 0.10f, 0.05f, 0.60f, 0.60f, 0.20f, 0.20f, 0.20f, 0.30f },
        { 280.0f, 40.0f, 255.0f, 0.75f, 0.50f, 0.38f, 0.50f, 0.20f, 0.10f, 0.80f, 0.40f, 0.40f, 0.30f, 0.10f, 0.10f },
        { 320.0f, -90.0f, 290.0f, 0.55f, 0.38f, 0.28f, 0.05f, 0.05f, 0.05f, 0.40f, 0.40f, 0.40f, 0.10f, 0.10f, 0.20f },
        { 210.0f, 180.0f, 270.0f, 0.82f, 0.60f, 0.50f, 0.60f, 0.50f, 0.20f, 0.20f, 0.20f, 0.50f, 0.15f, 0.15f, 0.25f },
        { -180.0f, 20.0f, 90.0f, 0.60f, 0.40f, 0.30f, 0.15f, 0.15f, 0.15f, 0.30f, 0.70f, 0.70f, 0.10f, 0.10f, 0.30f },
        { -210.0f, -50.0f, 85.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.70f, 0.30f, 0.30f, 0.25f, 0.25f, 0.40f },
        { -230.0f, 100.0f, 95.0f, 0.80f, 0.55f, 0.40f, 0.45f, 0.35f, 0.25f, 0.40f, 0.20f, 0.60f, 0.20f, 0.20f, 0.20f },
        { -260.0f, -120.0f, 80.0f, 0.72f, 0.48f, 0.36f, 0.30f, 0.20f, 0.10f, 0.20f, 0.60f, 0.20f, 0.40f, 0.20f },
        { -195.0f, 160.0f, 100.0f, 0.50f, 0.35f, 0.25f, 0.10f, 0.10f, 0.10f, 0.90f, 0.50f, 0.10f, 0.30f, 0.30f, 0.40f },
        { -300.0f, -20.0f, 90.0f, 0.75f, 0.50f, 0.38f, 0.55f, 0.45f, 0.35f, 0.30f, 0.30f, 0.80f, 0.10f, 0.10f, 0.30f },
        { -220.0f, 190.0f, 90.0f, 0.65f, 0.45f, 0.30f, 0.20f, 0.10f, 0.10f, 0.50f, 0.50f, 0.20f, 0.20f, 0.30f, 0.20f },
        { -280.0f, -180.0f, 110.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.60f, 0.20f, 0.20f, 0.15f, 0.15f, 0.25f },
        { -75.0f, 0.0f, 0.0f, 0.70f, 0.47f, 0.35f,  0.25f, 0.15f, 0.10f,  0.10f, 0.50f, 0.80f,  0.20f, 0.20f, 0.50f },
        { -63.0f, 8.0f, 140.0f, 0.80f, 0.55f, 0.40f,  0.10f, 0.10f, 0.10f,  0.90f, 0.20f, 0.20f,  0.15f, 0.15f, 0.25f },
        { -87.0f, 5.0f, 30.0f, 0.60f, 0.40f, 0.30f,  0.35f, 0.25f, 0.15f,  0.20f, 0.70f, 0.20f,  0.30f, 0.30f, 0.30f },
        { -73.0f, -10.0f, 200.0f, 0.75f, 0.50f, 0.38f,  0.20f, 0.20f, 0.20f,  0.50f, 0.50f, 0.50f,  0.10f, 0.10f, 0.10f },
        { 75.0f, 5.0f, 180.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.80f, 0.10f, 0.10f, 0.10f, 0.10f, 0.40f },
        { 85.0f, -5.0f, 210.0f, 0.80f, 0.55f, 0.40f, 0.10f, 0.10f, 0.10f, 0.20f, 0.60f, 0.20f, 0.20f, 0.30f, 0.20f },
        { 67.0f, 15.0f, 160.0f, 0.60f, 0.40f, 0.30f, 0.35f, 0.25f, 0.15f, 0.50f, 0.50f, 0.10f, 0.30f, 0.10f, 0.10f },
        { 63.0f, -15.0f, 300.0f, 0.50f, 0.35f, 0.25f, 0.10f, 0.05f, 0.05f, 0.70f, 0.30f, 0.80f, 0.15f, 0.15f, 0.20f },
        { 90.0f, 10.0f, 45.0f, 0.85f, 0.60f, 0.45f, 0.60f, 0.50f, 0.30f, 0.30f, 0.70f, 0.30f, 0.10f, 0.40f, 0.10f }
    };

    float steveWidth = 2.0f;
    float steveDepth = 2.0f;
    float steveHeight = 5.0f;

    for (int i = 0; i < 39; i++) {
        float steveX = steveData[i][0];
        float steveZ = steveData[i][1];

        collidableObjects.push_back({
            steveX - steveWidth / 2,
            0.0f,
            steveZ - steveDepth / 2,
            steveX + steveWidth / 2,
            steveHeight,
            steveZ + steveDepth / 2
            });
    }
    float length = 200, height = 800, depth = 200;
    float lx = length / 2.0f;
    float hy = height / 2.0f;
    float dz = depth / 2.0f;

    float blockXPos[6] = { -1560.0f, -1040.0f, -520.0f, 100.0f, 620.0f, 1140.0f };

    for (int block = 0; block < 6; block++) {
        float startX = blockXPos[block];

        if (block == 1 || block == 4) continue;

        for (int row = 0; row < 4; row++) {
            float tx1 = startX + lx;
            float tz1 = -600.0f - (row * 220.0f);

            collidableObjects.push_back({
                tx1 - lx, 0.0f, tz1 - dz,
                tx1 + lx, height, tz1 + dz
                });

            float tx2 = startX + lx + 220.0f;
            float tz2 = -600.0f - (row * 220.0f);

            collidableObjects.push_back({
                tx2 - lx, 0.0f, tz2 - dz,
                tx2 + lx, height, tz2 + dz
                });

            if (block == 0 || block == 5) {
                float tz3 = -600.0f - (row * 220.0f) + 1230.0f;

                collidableObjects.push_back({
                    tx1 - lx, 0.0f, tz3 - dz,
                    tx1 + lx, height, tz3 + dz
                    });

                collidableObjects.push_back({
                    tx2 - lx, 0.0f, tz3 - dz,
                    tx2 + lx, height, tz3 + dz
                    });
            }
        }
    }

    addAbrarCodeCollisions(collidableObjects);
    addCityCollisions(collidableObjects);

}

void handleKeypress(unsigned char key, int x, int y) {
    if (key == 'l' || key == 'L') {
        isLightOn = !isLightOn;
        glutPostRedisplay();
        updateAmbientSound();
        return;
    }

    if (abrarCode.getState() == STATE_WALKING) {
        float speed = 10.0f;
        float nextX = camX, nextY = camY, nextZ = camZ;

        switch (key) {
        case 'w': nextX += lookX * speed; nextZ += lookZ * speed; nextY += lookY * speed; break;
        case 's': nextX -= lookX * speed; nextZ -= lookZ * speed; nextY -= lookY * speed; break;
        case 'a': nextX -= (lookZ * -1.0f) * speed; nextZ -= lookX * speed; break;
        case 'd': nextX += (lookZ * -1.0f) * speed; nextZ += lookX * speed; break;
        case 'q': nextY -= 10.0f; break;
        case 'e': nextY += 10.0f; break;
        case 'g': case 'f': case 'n': case 'N': case 'm': case 'M':
            abrarCode.handleInput(key, camX, camY, camZ, collidableObjects);
            break;
        case 'r':   weatherstatus = (weatherstatus + 1) % 3;        updateAmbientSound(); // استدعاء الدالة عند تغيير الوقت
            break;
        case 'i':  cout << camX << " " << camY << " " << camZ << " " << lookX << " " << lookY << " " << lookZ << "\n"; break;
        case '1': switchCamera(0); break;
        case '2': switchCamera(1); break;
        case '3': switchCamera(2); break;
        case '4': switchCamera(3); break;
        case '5': switchCamera(4); break;
        case 27: exit(0); return;
        }

        saraCode.handleInput(key);

        if (nextY < GLOBAL_GROUND_LEVEL + PLAYER_BUFFER_Y) {
            nextY = GLOBAL_GROUND_LEVEL + PLAYER_BUFFER_Y;
        }

        bool isCurrentlyInside = isStrictlyInside(camX, camY, camZ);
        bool willBeInside = isStrictlyInside(nextX, nextY, nextZ);

        if ((!isCurrentlyInside && willBeInside) || (isCurrentlyInside && !willBeInside)) {
            //  PlaySound(TEXT("sounds/gallery.wav"), NULL, SND_ASYNC | SND_LOOP);

            if (camY < SHOWROOM_HEIGHT) {
                bool inGateWidth = (nextX > GATE_MIN_X && nextX < GATE_MAX_X);
                bool isLowEnough = (camY < 30.0f);
                if (!inGateWidth || !isLowEnough) {
                    glutPostRedisplay();
                    return;
                }
            }
        }


        if (willBeInside) {
            const float SECOND_FLOOR_Y_START = SHOWROOM_HEIGHT;
            const float SECOND_FLOOR_Y_END = SHOWROOM_HEIGHT + 60.0f;
            const float ELEV_X = -120.0f, ELEV_Z = -170.0f, ELEV_WIDTH = 30.0f;
            const float ELEV_MIN_X = ELEV_X - ELEV_WIDTH / 2.0f, ELEV_MAX_X = ELEV_X + ELEV_WIDTH / 2.0f;
            const float ELEV_MIN_Z = ELEV_Z - ELEV_WIDTH / 2.0f, ELEV_MAX_Z = ELEV_Z + ELEV_WIDTH / 2.0f;
            bool inElevatorShaft = (nextX > ELEV_MIN_X && nextX < ELEV_MAX_X && nextZ > ELEV_MIN_Z && nextZ < ELEV_MAX_Z);

            if (camY < SECOND_FLOOR_Y_START && nextY >= SECOND_FLOOR_Y_START) {
                if (!inElevatorShaft) { nextY = SECOND_FLOOR_Y_START - 0.1f; }
            }
            if (camY > SECOND_FLOOR_Y_START && nextY < SECOND_FLOOR_Y_START) {
                if (!inElevatorShaft) { nextY = SECOND_FLOOR_Y_START; }
            }
            if (camY < SECOND_FLOOR_Y_END && nextY >= SECOND_FLOOR_Y_END) {
                nextY = SECOND_FLOOR_Y_END - 0.1f;
            }
            if (camY >= SECOND_FLOOR_Y_END && nextY < SECOND_FLOOR_Y_END) {
                nextY = SECOND_FLOOR_Y_END;
            }
        }
        BoundingBox playerBox = { nextX - 1.5f, nextY - PLAYER_BUFFER_Y, nextZ - 1.5f, nextX + 1.5f, nextY + PLAYER_BUFFER_Y, nextZ + 1.5f };
        for (const auto& objBox : collidableObjects) {
            if (playerBox.maxX > objBox.minX && playerBox.minX < objBox.maxX &&
                playerBox.maxY > objBox.minY && playerBox.minY < objBox.maxY &&
                playerBox.maxZ > objBox.minZ && playerBox.minZ < objBox.maxZ) {
                glutPostRedisplay();
                return;
            }
        }

        camX = nextX;
        camY = nextY;
        camZ = nextZ;

        if (isCurrentlyInside != isStrictlyInside(camX, camY, camZ)) {
            updateAmbientSound();
        }
    }
    else {
        abrarCode.handleInput(key, camX, camY, camZ, collidableObjects);
        if (key == 27) exit(0);
    }

    glutPostRedisplay();
}


void handlePassiveMouse(int x, int y) {
    if (abrarCode.getState() == STATE_ENTERING) return;
    if (ignoreWarp) { ignoreWarp = false; return; }
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

void updateAmbientSound() {
    bool isInside = isStrictlyInside(camX, camY, camZ);

    if (isInside) {

        SoundManager::stopAll();
        SoundManager::playLoop("sounds/gallery.wav");
    }
    else {
        SoundManager::stopAll();
        if (weatherstatus == 2) {

            SoundManager::playLoop("sounds/rain.wav");
        }
        else if (isLightOn) {
            SoundManager::playLoop("sounds/morning.wav");
        }
        else {
            SoundManager::playLoop("sounds/night2.wav");
        }
    }
}

void updateAmbientSound1() {
    bool isInside = isStrictlyInside(camX, camY, camZ);

    if (isInside) {

        SoundManager::playLoopWithDelay("sounds/gallery.wav", 500);
    }
    else {
        if (weatherstatus == 2) {

            SoundManager::playLoopWithDelay("sounds/rain.wav", 500);
        }
        else if (isLightOn) {
            SoundManager::playLoopWithDelay("sounds/morning.wav", 500);
        }
        else {
            SoundManager::playLoopWithDelay("sounds/night2.wav", 500);
        }
    }
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

    dayskyTex = loadTextureFromFile("images\\\\daytexture2.2.png");
    nightskyTex = loadTextureFromFile("images\\\\nightsky.png");
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    myJeep.init();
    glassMgr.initShowroomWindows();
}

void initEnvironment() {}

void renderWorldContent() {


    abrarCode.drawCars();
    abrarCode.drawGroundFloorElevator();
    saraCode.drawAll();
    maher.draw(camX, camY, camZ, isLightOn, cctvTexIDs);
    abrarCode.drawSecondFloor();

    
    glPushMatrix();
    glTranslatef(0, -5, 0);
    glScalef(1, -1, 1);
    //abrarCode.drawCars();
    //abrarCode.drawGroundFloorElevator();
    //saraCode.drawAll();
    maher.draw(camX, camY, camZ, isLightOn, cctvTexIDs,0);
    abrarCode.drawSecondFloor(0);
    glPopMatrix();


    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat ambientDay[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat ambientNight[] = { 0.01f, 0.01f, 0.01f, 0.5f };
    GLfloat* currentAmbient = isLightOn ? ambientDay : ambientNight;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, currentAmbient);

    GLfloat noSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, noSpecular);

    for (int i = 1; i <= 3; i++) {
        myJeep.drawJeep(120.0f, 0.0f, 50.0f * i, 6.0f, 90.0f);
    }
    glPopAttrib();
}

void drawscene(bool drawcam = 1)
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

    if (weatherstatus == 2) myRain.draw();

    renderWorldContent();

    for (int i = 0; weatherstatus > 0 && i < CloudCount; i++) {
        myClouds[i].draw(weatherstatus);
    }

    if (drawcam && currentCam != 1) {
        glPushMatrix(); glTranslatef(minX, 60, maxz); glRotatef(270, 0, 1, 0);
        cam.setLookDirection(cameras[0].lookZ, cameras[0].lookY, -cameras[0].lookX); cam.draw(); glPopMatrix();
    }
    if (drawcam && currentCam != 2) {
        glPushMatrix(); glTranslatef(maxX, 60, maxz);
        cam.setLookDirection(cameras[1].lookX, cameras[1].lookY, cameras[1].lookZ); cam.draw(); glPopMatrix();
    }
    if (drawcam && currentCam != 3) {
        glPushMatrix(); glTranslatef(minX + 1, 60 - 1, maxz - 1); glRotatef(90, 0, 1, 0); glRotatef(90, 1, 0, 0);
        cam.setLookDirection(-cameras[2].lookZ, cameras[2].lookX, -cameras[2].lookY); cam.draw(); glPopMatrix();
    }
    if (drawcam && currentCam != 4) {
        glPushMatrix(); glTranslatef(maxX - 1, 60 - 1, minz + 1); glRotatef(270, 0, 1, 0); glRotatef(90, 1, 0, 0);
        cam.setLookDirection(cameras[3].lookZ, -cameras[3].lookX, -cameras[3].lookY); cam.draw(); glPopMatrix();
    }

    drawSkyDome(2000, 64, 64, skyTex);
}

/*
* glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat ambientDay[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat ambientNight[] = { 0.01f, 0.01f, 0.01f, 0.5f };
    GLfloat* currentAmbient = isLightOn ? ambientDay : ambientNight;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, currentAmbient);

    GLfloat noSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, noSpecular);

    for (int i = 1; i <= 3; i++) {
        myJeep.drawJeep(120.0f, 0.0f, 50.0f * i, 6.0f, 90.0f);
    }
    glPopAttrib();

    //drawSkyDome(2000, 64, 64, skyTex);

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

    if (weatherstatus == 2) myRain.draw();

    renderWorldContent();

    for (int i = 0; weatherstatus > 0 && i < CloudCount; i++) {
        myClouds[i].draw(weatherstatus);
    }
    glPushMatrix(); glTranslatef(minX, 60, maxz); glRotatef(270, 0, 1, 0);
    cam.setLookDirection(cameras[0].lookZ, cameras[0].lookY, -cameras[0].lookX); cam.draw(); glPopMatrix();
    glPushMatrix(); glTranslatef(maxX, 60, maxz);
    cam.setLookDirection(cameras[1].lookX, cameras[1].lookY, cameras[1].lookZ); cam.draw(); glPopMatrix();
    glPushMatrix(); glTranslatef(minX + 1, 60 - 1, maxz - 1); glRotatef(90, 0, 1, 0); glRotatef(90, 1, 0, 0);
    cam.setLookDirection(-cameras[2].lookZ, cameras[2].lookX, -cameras[2].lookY); cam.draw(); glPopMatrix();
    glPushMatrix(); glTranslatef(maxX - 1, 60 - 1, minz + 1); glRotatef(270, 0, 1, 0); glRotatef(90, 1, 0, 0);
    cam.setLookDirection(cameras[3].lookZ, -cameras[3].lookX, -cameras[3].lookY); cam.draw(); glPopMatrix();

    glPushMatrix();
    glTranslatef(-120.0f, 0.0f, -50.0f);
    glRotatef(90.0f, 0, 1, 0);
    glScalef(8.5f, 8.5f, 8.5f);
    carRed.draw();
    glPopMatrix();

    // Blue Car
    glPushMatrix();
    glTranslatef(-120.0f, 0.0f, -90.0f);
    glRotatef(90.0f, 0, 1, 0);
    glScalef(8.5f, 8.5f, 8.5f);
    carBlue.draw();
    glPopMatrix();

    // Yellow Car
    glPushMatrix();
    glTranslatef(-120.0f, 0.0f, -130.0f);
    glRotatef(90.0f, 0, 1, 0);
    glScalef(8.5f, 8.5f, 8.5f);
    carYellow.draw();
    glPopMatrix();

    drawscene();
*/
void ddref()
{
    return;
    glPushMatrix();
    glTranslatef(0, -5, 0);
    glScalef(1, -1, 1);

    abrarCode.drawCars();
    abrarCode.drawGroundFloorElevator();
    saraCode.drawAll();
    maher.draw(camX, camY, camZ, isLightOn, cctvTexIDs);
    abrarCode.drawSecondFloor();

    
    glPopMatrix();
}

void drawSceneForReflection() {
    
    GLuint skyTex = isLightOn ? dayskyTex : nightskyTex;
    drawSkyBody(isLightOn);
    glPushMatrix();
    glTranslatef(0, 5, 0);
    drawSkyDome(2000, 64, 64, skyTex);

    if (weatherstatus != 0)
        for (int i = 0; i < CloudCount; i++) myClouds[i].draw(weatherstatus);

    drawscene();
    glPopMatrix();
}

void display() {
    if (isLightOn) glClearColor(0.52f, 0.80f, 0.92f, 1.0f);
    else glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

    glEnable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    
    if (captureThisFrame) {
        int idx = lastCam - 1;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    setCamera(currentCam);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    //maher.drawGardenWater();
    maher.drawGardenReflection(drawSceneForReflection);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


    glDisable(GL_STENCIL_TEST);

    drawscene();

    // Red Car
    glPushMatrix();
    glTranslatef(-120.0f, 0.0f, -50.0f);
    glRotatef(90.0f, 0, 1, 0);
    glScalef(8.5f, 8.5f, 8.5f);
    carRed.draw();
    glPopMatrix();

    // Blue Car
    glPushMatrix();
    glTranslatef(-120.0f, 0.0f, -90.0f);
    glRotatef(90.0f, 0, 1, 0);
    glScalef(8.5f, 8.5f, 8.5f);
    carBlue.draw();
    glPopMatrix();

    // Yellow Car
    glPushMatrix();
    glTranslatef(-120.0f, 0.0f, -130.0f);
    glRotatef(90.0f, 0, 1, 0);
    glScalef(8.5f, 8.5f, 8.5f);
    carYellow.draw();
    glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    maher.drawGardenWater();

    glDisable(GL_BLEND);

    ddref();

    glassMgr.drawAll();

    glutSwapBuffers();
}

void handleResize(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 4000.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    srand(time(0));
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Showroom Simulator");

    initRendering();
    initParkingLotCars();
    createCollidables();
    initEnvironment();
    updateLookVector();
    updateAmbientSound();

    glutDisplayFunc(display);
    glutReshapeFunc(handleResize);
    glutKeyboardFunc(handleKeypress);
    glutPassiveMotionFunc(handlePassiveMouse);
    glutTimerFunc(16, update, 0);

    glutSetCursor(GLUT_CURSOR_NONE);
    glutMainLoop();
    return 0;
}