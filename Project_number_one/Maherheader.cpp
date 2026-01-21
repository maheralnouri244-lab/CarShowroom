#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <utility>
#include <string>
#include <cstdlib>
#include <ctime>
#include "WoodBuilding.h"
#include "Pillar.h"
#include "Hpillar.h"
#include "Showroomside.h"
#include "ShowroomGate.h"
#include "GlassWindow.h"
#include "PlusRing3D.h"
#include "Maherheader.h"
#include "StageLight.h"
#include "NeonTube.h"
#include "SteveModel.h"
#include "House.h"
#include "Tree.h"
#include "Showroomdoor.h"
#include "StreetLamp.h"
#include "MonitorScreen.h"
#include "OfficeDesk.h"
#include "CameraSphere.h"
#include "CityMasterPlan.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include\\stb_image.h"

using namespace std;

Maherheader::Maherheader() : showroomdoor(0, 15, 200) {
    for (int i = 0; i < 5; i++) houseTexID[i] = 0;
    groundTex = 0;
    roadTexID = 0;
    pavementTexID = 0;
    grassTexID = 0;
    RockTexID = 0;
    parkingTexID = 0;
    waterTexID = 0;
    woodTex = 0;
    roofTexID = 0;
    sideTexId = 0;
    for (int i=0;i<30;i++)
    build[i].generate();
}

void Maherheader::drawLightBeam(float x, float y, float z) {
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING);
    glTranslatef(x, y, z);
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    glColor4f(1.0f, 0.85f, 0.2f, 0.3f);
    glutSolidCone(30.0, 33.0, 32, 16);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glPopMatrix();
}

unsigned int Maherheader::loadTextureFromFile(const char* path) {
    /*static int x = 6;
    x--;
    if (x)
    return 0;*/
    int w, h, n;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &w, &h, &n, 4);
    if (!data) {
        return 0;
    }
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

void Maherheader::draw(float camX, float camY, float camZ, bool isday, GLuint cctvTexIDs[4]) {

    /*for (int i = 1; i < 5; i++) {
        string path = "images\\\\house" + to_string(i) + ".png";
        if (houseTexID[i] == 0) {
            houseTexID[i] = loadTextureFromFile(path.c_str());
        }
    }*/

    if (groundTex == 0)
        groundTex = loadTextureFromFile("images\\\\groundtexture.png");

    if (roadTexID == 0)      roadTexID = loadTextureFromFile("images\\\\road.png");
    if (pavementTexID == 0)  pavementTexID = loadTextureFromFile("images\\\\p2.png");
    if (grassTexID == 0)     grassTexID = loadTextureFromFile("images\\\\grassg.png");
    if (parkingTexID == 0)   parkingTexID = loadTextureFromFile("images\\\\parking.jpg");
    if (waterTexID == 0)     waterTexID = loadTextureFromFile("images\\\\water.png");

    if (grassTexID == 0) grassTexID = groundTex;
    if (parkingTexID == 0) parkingTexID = roadTexID;
    if (woodTex==0) woodTex= loadTextureFromFile("images\\\\wood.png");

    if (sideTexId==0) sideTexId= loadTextureFromFile("images\\buildTex.jpg"); 
    if (roofTexID==0) roofTexID= loadTextureFromFile("images\\roof.png");
    if (RockTexID==0) RockTexID= loadTextureFromFile("images\\Rockground5.png");
    if (treeLeafTexID == 0) treeLeafTexID = loadTextureFromFile("images\\Leaves.png");
    if (treeBarkTexID == 0) treeBarkTexID = loadTextureFromFile("images\\wood.png");

    // =========================================================
    // رسم تخطيط المدينة (الشوارع، الحديقة، المواقف)
    // =========================================================
    cityPlan.setTextures(roadTexID, pavementTexID, grassTexID, parkingTexID, waterTexID, pavementTexID, treeLeafTexID, treeBarkTexID);
    cityPlan.drawCityLayout(!isday);

    PlusRing3D symbol;
    vector<Pillar> pillars;
    vector<Hpillar> hpillars;
    vector<Showroomside> showroomsides;
    vector <NeonTube> neonyubes;
    float maxX = 150, maxz = 200, diff = 45;
    float minX = -maxX, minz = -maxz;
    pair<float, float> p[4] = { {maxX,maxz},{minX,maxz},{maxX,minz},{minX,minz} };
    int h = 60;

    for (int i = 0; i < 1; i++) {
        pillars.push_back(Pillar(p[i].first, 0, p[i].second, h));
    }

    hpillars.push_back(Hpillar(maxX, 0, 0, 0.4, 0.8, maxz - minz));
    hpillars.push_back(Hpillar(maxX, h - 0.4, 0, 0.4, 0.8, maxz - minz));
    hpillars.push_back(Hpillar(minX, 0, 0, 0.4, 0.8, maxz - minz));
    hpillars.push_back(Hpillar(minX, h - 0.4, 0, 0.4, 0.8, maxz - minz));
    hpillars.push_back(Hpillar(0, 0, minz, 0.4, maxX - minX, 0.8));
    hpillars.push_back(Hpillar(0, h - 0.4, minz, 0.4, maxX - minX, 0.8));
    hpillars.push_back(Hpillar(0, h - 0.4, maxz, 0.4, maxX - minX, 0.8));
    hpillars.push_back(Hpillar(minX + (maxX - minX - diff) / 4, 0, maxz, 0.4, (maxX - minX - diff) / 2, 0.8));
    hpillars.push_back(Hpillar(maxX - (maxX - minX - diff) / 4, 0, maxz, 0.4, (maxX - minX - diff) / 2, 0.8));

    hpillars.push_back(Hpillar(maxX - (maxX - minX - diff) / 2, 0, 0, 0.4, 0.8, maxz - minz - 2 * diff));
    hpillars.push_back(Hpillar(minX + (maxX - minX - diff) / 2, 0, 0, 0.4, 0.8, maxz - minz - 2 * diff));
    hpillars.push_back(Hpillar(maxX - (maxX - minX - diff) / 2, h, 0, 0.4, 0.8, maxz - minz - 2 * diff));
    hpillars.push_back(Hpillar(minX + (maxX - minX - diff) / 2, h, 0, 0.4, 0.8, maxz - minz - 2 * diff));

    showroomsides.push_back(Showroomside(maxX, minX, minz, minz, h, 0.2, 0.2, 0.2, 40, 5));
    showroomsides.push_back(Showroomside(minX, minX, maxz, minz, h, 0.2, 0.2, 0.2, 40, 5));
    showroomsides.push_back(Showroomside(maxX, maxX, maxz, minz, h, 0.2, 0.2, 0.2, 40, 5));
    showroomsides.push_back(Showroomside(minX + (maxX - minX - diff) / 2, minX, maxz, maxz, h, 0.2, 0.2, 0.2, 18, 5));
    showroomsides.push_back(Showroomside(maxX, maxX - (maxX - minX - diff) / 2, maxz, maxz, h, 0.2, 0.2, 0.2, 18, 5));

    showroomsides.push_back(Showroomside(minX + (maxX - minX - diff) / 2, minX + (maxX - minX - diff) / 2, (maxz - minz - 2 * diff) / 2, -(maxz - minz - 2 * diff) / 2, h, 0.2, 0.2, 0.2, 18, 5));
    showroomsides.push_back(Showroomside(maxX - (maxX - minX - diff) / 2, maxX - (maxX - minX - diff) / 2, (maxz - minz - 2 * diff) / 2, -(maxz - minz - 2 * diff) / 2, h, 0.2, 0.2, 0.2, 18, 5));

    neonyubes.push_back(NeonTube(minX + (maxX - minX - diff) / 2 + 4, 0, 0, maxz - minz - diff, 0.1, 'z'));
    neonyubes.push_back(NeonTube(maxX - (maxX - minX - diff) / 2 - 4, 0, 0, maxz - minz - diff, 0.1, 'z'));
    neonyubes.push_back(NeonTube(0, 0, (maxz - minz - diff) / 2, (maxX - minX - diff / 2), 0.1, 'x'));
    neonyubes.push_back(NeonTube(0, 0, -(maxz - minz - diff) / 2, (maxX - minX - diff / 2), 0.1, 'x'));
    neonyubes.push_back(NeonTube(minX + (maxX - minX - diff) / 4, 0, (maxz + minz) / 2 + 2 * diff, (maxz - minz) / 2 - 2 * diff, 0.1, 'z'));
    neonyubes.push_back(NeonTube(minX + (maxX - minX - diff) / 4, 0, -(maxz + minz) / 2 - 2 * diff, (maxz - minz) / 2 - 2 * diff, 0.1, 'z'));
    neonyubes.push_back(NeonTube(-minX - (maxX - minX - diff) / 4, 0, (maxz + minz) / 2 + 2 * diff, (maxz - minz) / 2 - 2 * diff, 0.1, 'z'));
    neonyubes.push_back(NeonTube(-minX - (maxX - minX - diff) / 4, 0, -(maxz + minz) / 2 - 2 * diff, (maxz - minz) / 2 - 2 * diff, 0.1, 'z'));

    for (const auto& p : pillars) p.draw();
    for (const auto& p : hpillars) p.draw();
    for (const auto& p : showroomsides) p.draw();
    for (const auto& p : neonyubes) p.draw();

    ShowroomGate showroomgate;
    showroomgate.draw();

    glPushMatrix();
    glTranslatef(0, 0, (maxz - minz - 2 * diff) / 2);
    showroomgate.drawGateArch();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -(maxz - minz - 2 * diff) / 2);
    showroomgate.drawGateArch();
    glPopMatrix();

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(maxX, h, maxz);
    glVertex3f(maxX, h, minz);
    glVertex3f(minX, h, minz);
    glVertex3f(minX, h, maxz);
    glEnd();

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(maxX, 0, maxz);
    glVertex3f(maxX, 0, minz);
    glVertex3f(minX, 0, minz);
    glVertex3f(minX, 0, maxz);
    glEnd();

    int cnt = 5;
    for (int i = 0; i <= cnt; i++) {
        NeonTube C1 = NeonTube(-minX - (maxX - minX - diff) / 4, (30.0 / cnt) * i, 0, (maxz - minz) / 2 - 2 * diff, 0.1, 'z');
        C1.drawC(0.2, 40);
        if (i == 0) C1.drawColumns(40, 12, 30, 0.2, 3, 1);
    }
    for (int i = 0; i <= cnt; i++) {
        NeonTube C2 = NeonTube(minX + (maxX - minX - diff) / 4, (30.0 / cnt) * i, 0, (maxz - minz) / 2 - 2 * diff, 0.1, 'z');
        C2.drawC(0.2, 40);
        if (i == 0) C2.drawColumns(40, 12, 30, 0.2, 3, 1);
    }

    // رسم شخصيات ستيف (Steves)
    float groupBaseX = minX + (maxX - minX - diff) / 4.0f;
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
        { groupBaseX + 0.0f,   0.0f,   0.0f,   0.70f, 0.47f, 0.35f,  0.25f, 0.15f, 0.10f,  0.10f, 0.50f, 0.80f,  0.20f, 0.20f, 0.50f },
        { groupBaseX + 12.0f,  8.0f,   140.0f, 0.80f, 0.55f, 0.40f,  0.10f, 0.10f, 0.10f,  0.90f, 0.20f, 0.20f,  0.15f, 0.15f, 0.25f },
        { groupBaseX - 12.0f,  5.0f,   30.0f,  0.60f, 0.40f, 0.30f,  0.35f, 0.25f, 0.15f,  0.20f, 0.70f, 0.20f,  0.30f, 0.30f, 0.30f },
        { groupBaseX + 2.0f,  -10.0f,  200.0f, 0.75f, 0.50f, 0.38f,  0.20f, 0.20f, 0.20f,  0.50f, 0.50f, 0.50f,  0.10f, 0.10f, 0.10f },
        { -groupBaseX + 0.0f,   5.0f,   180.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.80f, 0.10f, 0.10f, 0.10f, 0.10f, 0.40f },
        { -groupBaseX + 10.0f, -5.0f,   210.0f, 0.80f, 0.55f, 0.40f, 0.10f, 0.10f, 0.10f, 0.20f, 0.60f, 0.20f, 0.20f, 0.30f, 0.20f },
        { -groupBaseX - 8.0f,  15.0f,   160.0f, 0.60f, 0.40f, 0.30f, 0.35f, 0.25f, 0.15f, 0.50f, 0.50f, 0.10f, 0.30f, 0.10f, 0.10f },
        { -groupBaseX - 12.0f, -15.0f,  300.0f, 0.50f, 0.35f, 0.25f, 0.10f, 0.05f, 0.05f, 0.70f, 0.30f, 0.80f, 0.15f, 0.15f, 0.20f },
        { -groupBaseX + 15.0f, 10.0f,   45.0f,  0.85f, 0.60f, 0.45f, 0.60f, 0.50f, 0.30f, 0.30f, 0.70f, 0.30f, 0.10f, 0.40f, 0.10f },
    };

    SteveModel steve(0.3f);
    for (int i = 0; i < 39; i++) {
        SteveModel::setSkinColor(steveData[i][3], steveData[i][4], steveData[i][5]);
        SteveModel::setHairColor(steveData[i][6], steveData[i][7], steveData[i][8]);
        SteveModel::setShirtColor(steveData[i][9], steveData[i][10], steveData[i][11]);
        SteveModel::setPantsColor(steveData[i][12], steveData[i][13], steveData[i][14]);
        glPushMatrix();
        glTranslatef(steveData[i][0], 0.0f, steveData[i][1]);
        glRotatef(steveData[i][2], 0.0f, 1.0f, 0.0f);
        steve.draw();
        glPopMatrix();
    }

    showroomdoor.draw();
    if (sqrt((pow(camX - 0, 2) + pow(camY - 0, 2) + pow(camZ - maxz, 2))) <= 100.0)
        showroomdoor.moveUp(1);
    else
        showroomdoor.moveDown(1);

    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(0, h + 1, 0);
    glScalef(2 * maxX, 1, 2 * maxz);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, h + 3, maxz + 6);
    glScalef(14, 14, 14);
    symbol.draw();
    glPopMatrix();

    MonitorScreen myMonitor;
    OfficeDesk mydisk;
    glPushMatrix();
    glTranslatef(minX + 30, h + 16, maxz - 23);
    glRotatef(90, 0, 1, 0);
    myMonitor.draw(cctvTexIDs);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(minX + 30, h - 6 + 6.5, maxz - 25);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(90, 0, 1, 0);
    mydisk.draw();
    glPopMatrix();


    float housepos[30][3] = {
     {-412.3f, 0.0f, -512.7f},
    {-125.8f, 0.0f, -745.2f},
    {-689.4f, 0.0f, -328.1f},
    {-233.6f, 0.0f, -902.4f},
    {-842.1f, 0.0f, -456.9f},
    {-321.7f, 0.0f, -612.5f},
    {-578.2f, 0.0f, -733.8f},
    {-95.4f,  0.0f, -845.6f},
    {-710.9f, 0.0f, -389.2f},
    {-268.5f, 0.0f, -654.7f},
    {-490.6f, 0.0f, -812.3f},
    {-152.7f, 0.0f, -905.1f},
    {-623.8f, 0.0f, -478.2f},
    {-341.9f, 0.0f, -701.4f},
    {-812.5f, 0.0f, -529.6f},
    {-210.4f, 0.0f, -832.7f},
    {-455.2f, 0.0f, -623.9f},
    {-134.8f, 0.0f, -764.2f},
    {-689.7f, 0.0f, -905.3f},
    {-278.1f, 0.0f, -512.6f},
    {-512.9f, 0.0f, -689.4f},
    {-189.3f, 0.0f, -845.7f},
    {-623.4f, 0.0f, -398.2f},
    {-341.7f, 0.0f, -732.9f},
    {-812.8f, 0.0f, -612.4f},
    {-210.6f, 0.0f, -823.5f},
    {-455.6f, 0.0f, -701.8f},
    {-134.5f, 0.0f, -764.9f},
    {-689.9f, 0.0f, -905.7f},
    {-278.4f, 0.0f, -523.1f},
    };

    
    float length = 200, height = 800, depth = 200; float lx = length / 2.0f; float hy = height / 2.0f; float dz = depth / 2.0f; float repW = 2.0f; float repH = 4.0f;
    float fullBlockWidth = 420;
    float blockXPos[6]; blockXPos[3] = 100.0f; blockXPos[4] = 620.0f; blockXPos[5] = 1140.0f; blockXPos[2] = -520.0f; blockXPos[1] = -1040.0f; blockXPos[0] = -1560.0f;

    glEnable(GL_TEXTURE_2D); glColor3f(1.0f, 1.0f, 1.0f);

    for (int block = 0; block < 6; block++) {
        float startX = blockXPos[block];

        glPushMatrix();
        glTranslatef(startX + 210, 0.2, -600 - 430+100);
        glBindTexture(GL_TEXTURE_2D, pavementTexID);
        glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0);        glVertex3f(210, 0, 530);
        glTexCoord2f(50, 0);     glVertex3f(210, 0, -1070);
        glTexCoord2f(50, 15);  glVertex3f(-210, 0, -1070);
        glTexCoord2f(0, 15);     glVertex3f(-210, 0, 530);
        glEnd();
        glPopMatrix();


        if (block == 1 || block == 4)
            continue;
        for (int row = 0; row < 4; row++) {
            glPushMatrix();
            glTranslatef(startX + lx, 0, -600 - (row * 220));
            glBindTexture(GL_TEXTURE_2D, sideTexId);
            glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);
            glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, dz);
            glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
            glTexCoord2f(repW, repH); glVertex3f(lx, hy, dz);
            glTexCoord2f(0, repH);    glVertex3f(-lx, hy, dz);
            glNormal3f(0, 0, -1);
            glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, -dz);
            glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, -dz);
            glTexCoord2f(repW, repH); glVertex3f(lx, hy, -dz);
            glTexCoord2f(0, repH);    glVertex3f(-lx, hy, -dz);
            glNormal3f(-1, 0, 0);
            glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, -dz);
            glTexCoord2f(repW, 0);    glVertex3f(-lx, -hy, dz);
            glTexCoord2f(repW, repH); glVertex3f(-lx, hy, dz);
            glTexCoord2f(0, repH);    glVertex3f(-lx, hy, -dz);
            glNormal3f(1, 0, 0);
            glTexCoord2f(0, 0);       glVertex3f(lx, -hy, -dz);
            glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
            glTexCoord2f(repW, repH); glVertex3f(lx, hy, dz);
            glTexCoord2f(0, repH);    glVertex3f(lx, hy, -dz);
            glNormal3f(0, -1, 0);
            glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, dz);
            glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
            glTexCoord2f(repW, repW); glVertex3f(lx, -hy, -dz);
            glTexCoord2f(0, repW);    glVertex3f(-lx, -hy, -dz);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, roofTexID);
            glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glTexCoord2f(0, 0);       glVertex3f(-lx, hy, dz);
            glTexCoord2f(1, 0);    glVertex3f(lx, hy, dz);
            glTexCoord2f(1,1); glVertex3f(lx, hy, -dz);
            glTexCoord2f(0, 1);    glVertex3f(-lx, hy, -dz);
            glEnd();
            glPopMatrix();
        }

        for (int row = 0; row < 4; row++) {
            glPushMatrix();
            glTranslatef(startX + lx + 220, 0, -600 - (row * 220));
            glBindTexture(GL_TEXTURE_2D, sideTexId);
            glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);
            glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, dz);
            glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
            glTexCoord2f(repW, repH); glVertex3f(lx, hy, dz);
            glTexCoord2f(0, repH);    glVertex3f(-lx, hy, dz);
            glNormal3f(0, 0, -1);
            glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, -dz);
            glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, -dz);
            glTexCoord2f(repW, repH); glVertex3f(lx, hy, -dz);
            glTexCoord2f(0, repH);    glVertex3f(-lx, hy, -dz);
            glNormal3f(-1, 0, 0);
            glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, -dz);
            glTexCoord2f(repW, 0);    glVertex3f(-lx, -hy, dz);
            glTexCoord2f(repW, repH); glVertex3f(-lx, hy, dz);
            glTexCoord2f(0, repH);    glVertex3f(-lx, hy, -dz);
            glNormal3f(1, 0, 0);
            glTexCoord2f(0, 0);       glVertex3f(lx, -hy, -dz);
            glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
            glTexCoord2f(repW, repH); glVertex3f(lx, hy, dz);
            glTexCoord2f(0, repH);    glVertex3f(lx, hy, -dz);
            glNormal3f(0, -1, 0);
            glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, dz);
            glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
            glTexCoord2f(repW, repW); glVertex3f(lx, -hy, -dz);
            glTexCoord2f(0, repW);    glVertex3f(-lx, -hy, -dz);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, roofTexID);
            glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glTexCoord2f(0, 0);       glVertex3f(-lx, hy, dz);
            glTexCoord2f(1, 0);    glVertex3f(lx, hy, dz);
            glTexCoord2f(1, 1); glVertex3f(lx, hy, -dz);
            glTexCoord2f(0, 1);    glVertex3f(-lx, hy, -dz);
            glEnd();
            glPopMatrix();
        }

        if (block == 0 || block == 5)
        {
            glPushMatrix();
            glTranslatef(startX + 210, 0.2, -600 - 430 + 100+1230);
            glBindTexture(GL_TEXTURE_2D, pavementTexID);
            glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glTexCoord2f(0, 0);        glVertex3f(230, 0, 530);
            glTexCoord2f(50, 0);     glVertex3f(230, 0, -530);
            glTexCoord2f(50, 15);  glVertex3f(-230, 0, -530);
            glTexCoord2f(0, 15);     glVertex3f(-230, 0, 530);
            glEnd();
            glPopMatrix();


            for (int row = 0; row < 4; row++) {
                glPushMatrix();
                glTranslatef(startX + lx, 0, -600 - (row * 220) + 1230);
                glBindTexture(GL_TEXTURE_2D, sideTexId);
                glBegin(GL_QUADS);
                glNormal3f(0, 0, 1);
                glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, dz);
                glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
                glTexCoord2f(repW, repH); glVertex3f(lx, hy, dz);
                glTexCoord2f(0, repH);    glVertex3f(-lx, hy, dz);
                glNormal3f(0, 0, -1);
                glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, -dz);
                glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, -dz);
                glTexCoord2f(repW, repH); glVertex3f(lx, hy, -dz);
                glTexCoord2f(0, repH);    glVertex3f(-lx, hy, -dz);
                glNormal3f(-1, 0, 0);
                glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, -dz);
                glTexCoord2f(repW, 0);    glVertex3f(-lx, -hy, dz);
                glTexCoord2f(repW, repH); glVertex3f(-lx, hy, dz);
                glTexCoord2f(0, repH);    glVertex3f(-lx, hy, -dz);
                glNormal3f(1, 0, 0);
                glTexCoord2f(0, 0);       glVertex3f(lx, -hy, -dz);
                glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
                glTexCoord2f(repW, repH); glVertex3f(lx, hy, dz);
                glTexCoord2f(0, repH);    glVertex3f(lx, hy, -dz);
                glNormal3f(0, -1, 0);
                glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, dz);
                glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
                glTexCoord2f(repW, repW); glVertex3f(lx, -hy, -dz);
                glTexCoord2f(0, repW);    glVertex3f(-lx, -hy, -dz);
                glEnd();
                glBindTexture(GL_TEXTURE_2D, roofTexID);
                glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);
                glTexCoord2f(0, 0);       glVertex3f(-lx, hy, dz);
                glTexCoord2f(1, 0);    glVertex3f(lx, hy, dz);
                glTexCoord2f(1, 1); glVertex3f(lx, hy, -dz);
                glTexCoord2f(0, 1);    glVertex3f(-lx, hy, -dz);
                glEnd();
                glPopMatrix();
            }

            for (int row = 0; row < 4; row++) {
                glPushMatrix();
                glTranslatef(startX + lx + 220, 0, -600 - (row * 220) + 1230);
                glBindTexture(GL_TEXTURE_2D, sideTexId);
                glBegin(GL_QUADS);
                glNormal3f(0, 0, 1);
                glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, dz);
                glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
                glTexCoord2f(repW, repH); glVertex3f(lx, hy, dz);
                glTexCoord2f(0, repH);    glVertex3f(-lx, hy, dz);
                glNormal3f(0, 0, -1);
                glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, -dz);
                glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, -dz);
                glTexCoord2f(repW, repH); glVertex3f(lx, hy, -dz);
                glTexCoord2f(0, repH);    glVertex3f(-lx, hy, -dz);
                glNormal3f(-1, 0, 0);
                glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, -dz);
                glTexCoord2f(repW, 0);    glVertex3f(-lx, -hy, dz);
                glTexCoord2f(repW, repH); glVertex3f(-lx, hy, dz);
                glTexCoord2f(0, repH);    glVertex3f(-lx, hy, -dz);
                glNormal3f(1, 0, 0);
                glTexCoord2f(0, 0);       glVertex3f(lx, -hy, -dz);
                glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
                glTexCoord2f(repW, repH); glVertex3f(lx, hy, dz);
                glTexCoord2f(0, repH);    glVertex3f(lx, hy, -dz);
                glNormal3f(0, -1, 0);
                glTexCoord2f(0, 0);       glVertex3f(-lx, -hy, dz);
                glTexCoord2f(repW, 0);    glVertex3f(lx, -hy, dz);
                glTexCoord2f(repW, repW); glVertex3f(lx, -hy, -dz);
                glTexCoord2f(0, repW);    glVertex3f(-lx, -hy, -dz);
                glEnd();
                glBindTexture(GL_TEXTURE_2D, roofTexID);
                glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);
                glTexCoord2f(0, 0);       glVertex3f(-lx, hy, dz);
                glTexCoord2f(1, 0);    glVertex3f(lx, hy, dz);
                glTexCoord2f(1, 1); glVertex3f(lx, hy, -dz);
                glTexCoord2f(0, 1);    glVertex3f(-lx, hy, -dz);
                glEnd();
                glPopMatrix();
            }
        }
    } 
    
    glPushMatrix();
    glTranslatef(-1560-220, 0.2, -1200 + 1230);
    glBindTexture(GL_TEXTURE_2D, pavementTexID);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0);        glVertex3f(220, 0, 800);
    glTexCoord2f(50, 0);     glVertex3f(220, 0, -270);
    glTexCoord2f(50, 15);  glVertex3f(-220, 0, -270);
    glTexCoord2f(0, 15);     glVertex3f(-220, 0, 800);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1560 + 220, 0.2, -1200+ 1230);
    glBindTexture(GL_TEXTURE_2D, pavementTexID);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0);        glVertex3f(220, 0, 800);
    glTexCoord2f(50, 0);     glVertex3f(220, 0, -270);
    glTexCoord2f(50, 15);  glVertex3f(-220, 0, -270);
    glTexCoord2f(0, 15);     glVertex3f(-220, 0, 800);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1560 - 230, 0.2, -1200);
    glBindTexture(GL_TEXTURE_2D, pavementTexID);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0);        glVertex3f(210, 0, 800);
    glTexCoord2f(50, 0);     glVertex3f(210, 0, -800);
    glTexCoord2f(50, 15);  glVertex3f(-210, 0, -800);
    glTexCoord2f(0, 15);     glVertex3f(-210, 0, 800);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1560 + 230, 0.2, -1200);
    glBindTexture(GL_TEXTURE_2D, pavementTexID);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0);        glVertex3f(210, 0, 800);
    glTexCoord2f(50, 0);     glVertex3f(210, 0, -800);
    glTexCoord2f(50, 15);  glVertex3f(-210, 0, -800);
    glTexCoord2f(0, 15);     glVertex3f(-210, 0, 800);
    glEnd();
    glPopMatrix();


    glDisable(GL_TEXTURE_2D);
    cityPlan.drawRoadSegment(0.0f, 0.05f, -2000.0f, 160.0f, 1600.0f, false);
    cityPlan.drawSidewalk(90.0f, -1200.0f, 20.0f, 1600.0f, 0.5f);
    cityPlan.drawSidewalk(-90.0f, -1200.0f, 20.0f, 1600.0f, 0.5f);

    float roadX_01 = blockXPos[0] + fullBlockWidth + 50.0f;
    cityPlan.drawRoadSegment(roadX_01, 0.05f, -2000.0f, 70.0f, 1600.0f, false);
    cityPlan.drawSidewalk(roadX_01 + 42.5f, -1200.0f, 15.0f, 1600.0f, 0.5f);
    cityPlan.drawSidewalk(roadX_01 - 42.5f, -1200.0f, 15.0f, 1600.0f, 0.5f);

    float roadX_12 = blockXPos[1] + fullBlockWidth + 50.0f;
    cityPlan.drawRoadSegment(roadX_12, 0.05f, -2000.0f, 70.0f, 1600.0f, false);
    cityPlan.drawSidewalk(roadX_12 + 42.5f, -1200.0f, 15.0f, 1600.0f, 0.5f);
    cityPlan.drawSidewalk(roadX_12 - 42.5f, -1200.0f, 15.0f, 1600.0f, 0.5f);

    float roadX_34 = blockXPos[3] + fullBlockWidth + 50.0f;
    cityPlan.drawRoadSegment(roadX_34, 0.05f, -2000.0f, 70.0f, 1600.0f, false);
    cityPlan.drawSidewalk(roadX_34 + 42.5f, -1200.0f, 15.0f, 1600.0f, 0.5f);
    cityPlan.drawSidewalk(roadX_34 - 42.5f, -1200.0f, 15.0f, 1600.0f, 0.5f);

    float roadX_45 = blockXPos[4] + fullBlockWidth + 50.0f;
    cityPlan.drawRoadSegment(roadX_45, 0.05f, -2000.0f, 70.0f, 1600.0f, false);
    cityPlan.drawSidewalk(roadX_45 + 42.5f, -1200.0f, 15.0f, 1600.0f, 0.5f);
    cityPlan.drawSidewalk(roadX_45 - 42.5f, -1200.0f, 15.0f, 1600.0f, 0.5f);

    cityPlan.drawRoadSegment(-2000,0.1,-320,160,4000,true);
    //cityPlan.drawSidewalk()
    cityPlan.drawSidewalk(0 , -240.0f, 4000.0f, 20.0f, 2);

}