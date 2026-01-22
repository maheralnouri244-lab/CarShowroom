#include "CityMasterPlan.h"
#include "Sedan.h"
#include "SUV.h"
#include "Taxi.h"
#include "SportsCar.h"
#include "Ambulance.h"
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void setupRepeatedTexture() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

CityMasterPlan::CityMasterPlan() : streetLightObj(22.0f, 0.7f) {
    roadTex = 0; 
    pavementTex = 0; 
    grassTex = 0; 
    parkingTex = 0; 
    waterTex = 0; 
    wallTex = 0;
}

void CityMasterPlan::setTextures(unsigned int road, unsigned int pavement, unsigned int grass, unsigned int parkingAsphalt, unsigned int water, unsigned int wall, unsigned int treeBark, unsigned int treeLeaf) {
    roadTex = road; 
    pavementTex = pavement; 
    grassTex = grass; 
    parkingTex = parkingAsphalt; 
    waterTex = water; 
    wallTex = wall;

    roadSys.setTextures(road, pavement, parkingAsphalt, grass);
    vegetation.setTextures(treeBark, treeLeaf);
    myGarden.setTextures(pavement, grass, wall, parkingAsphalt);

}

void CityMasterPlan::drawStreetLightRow(float x, float startZ, float spacing, int count, bool isRightSide, bool isNight) {
    for (int i = 0; i < count; i++) {
        glPushMatrix();
        glTranslatef(x, 0, startZ + i * spacing);
        if (isRightSide) 
            glRotatef(-90, 0, 1, 0);
        else 
            glRotatef(90, 0, 1, 0);
        streetLightObj.draw(isNight);
        glPopMatrix();
    }
}

void CityMasterPlan::createAndDrawRandomCar(float x, float y, float z, float rotation, int seed) {
    int type = seed % 10;
    float r = (seed % 10) / 10.0f; 
    float g = ((seed * 3) % 10) / 10.0f; 
    float b = ((seed * 7) % 10) / 10.0f;

    if (r < 0.2 && g < 0.2 && b < 0.2)
        r = 0.8f;

    Car* car = nullptr;
    if (type < 4) 
        car = new Sedan(x, y, z, rotation, r, g, b);
    else if (type < 6) 
        car = new SUV(x, y, z, rotation, r, g, b);
    else if (type < 8) 
        car = new SportsCar(x, y, z, rotation, 1.0f, 0.1f, 0.1f);
    else if (type == 8) 
        car = new Taxi(x, y, z, rotation);
    else 
        car = new Ambulance(x, y, z, rotation);

    if (car) {
        car->draw();
        delete car;
    }
}

void CityMasterPlan::drawCityLayout(bool isNight) {
    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);

    glEnable(GL_CULL_FACE); glCullFace(GL_BACK);

    roadSys.drawInfiniteGround();

    buildShowroomPlaza();

    glDisable(GL_CULL_FACE);
    structures.drawShowroomSign(isNight);
    glEnable(GL_CULL_FACE);

    buildMainBoulevard(isNight);

    glPushMatrix(); 
    glTranslatef(0, 0, 600.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roadTex); 
    setupRepeatedTexture(); 
    glColor3f(1, 1, 1);
    int segs = 64; 
    glBegin(GL_QUAD_STRIP); 
    for (int i = 0; i <= segs; i++) {
        float a = (float)i / segs * 2 * M_PI; 
        float r = 80, rO = 120; 
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, (float)i / 5);
        glVertex3f(r * cos(a), 0.05, r * sin(a)); 
        glTexCoord2f(1, (float)i / 5);
        glVertex3f(rO * cos(a), 0.05, rO * sin(a));
    } 
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pavementTex);
    setupRepeatedTexture(); 
    glBegin(GL_TRIANGLE_FAN); 
    glNormal3f(0, 1, 0); 
    glTexCoord2f(0.5, 0.5); 
    glVertex3f(0, 0.5, 0);
    for (int i = 0; i <= segs; i++) {
        float a = (float)i / segs * 2 * M_PI; 
        glTexCoord2f(0.5 + 0.5 * cos(a), 0.5 + 0.5 * sin(a)); 
        glVertex3f(78 * cos(a), 0.5, 78 * sin(a)); 
    } 
    glEnd(); 
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glScalef(10, 20, 10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 15, 0);
    glutSolidSphere(8, 20, 20);
    glPopMatrix();

    vegetation.drawFlowerBed(15, 0.5f, 0);
    vegetation.drawFlowerBed(-15, 0.5f, 0);

    float oR = 120.0f;
    roadSys.drawCurvedSidewalk(oR + 10, 0.5f, oR + 10, oR, M_PI, 1.5 * M_PI, 20);
    roadSys.drawCurvedSidewalk(-(oR + 10), 0.5f, oR + 10, oR, 1.5 * M_PI, 2 * M_PI, 20);
    roadSys.drawCurvedSidewalk(oR + 10, 0.5f, -(oR + 10), oR, 0.5 * M_PI, M_PI, 20);
    roadSys.drawCurvedSidewalk(-(oR + 10), 0.5f, -(oR + 10), oR, 0, 0.5 * M_PI, 20);
    glPopMatrix();

    float outerEdge = 120.0f;
    roadSys.drawRoadSegment(outerEdge + 500.0f, 0.051f, 600.0f, 1000.0f, 60.0f, true);
    roadSys.drawRoadSegment(-(outerEdge + 500.0f), 0.051f, 600.0f, 1000.0f, 60.0f, true);

    buildParkingZone(isNight);
    buildCommercialZone();
    drawLakeParkArea(isNight);

    glPopAttrib();
    glDisable(GL_TEXTURE_2D); glColor3f(1, 1, 1);
}

void CityMasterPlan::drawLakeParkArea(bool isNight) {
    float parkX = -280.0f; 
    float parkZ = 600.0f;

    myGarden.draw(parkX, 0.0f, parkZ, isNight);

    structures.drawGazebo(parkX + 100, 0.1f, parkZ - 150);
    structures.drawJuiceStall(parkX - 100, 0.1f, parkZ - 150);

    vegetation.drawTree(parkX - 150, 0.1f, parkZ - 200);
    vegetation.drawTree(parkX + 150, 0.1f, parkZ + 200);
    vegetation.drawTree(parkX - 150, 0.1f, parkZ + 200);
    vegetation.drawTree(parkX + 150, 0.1f, parkZ - 200);
    vegetation.drawTree(parkX, 0.1f, parkZ - 220);
}

void CityMasterPlan::buildShowroomPlaza() {
    float srMinX = -150, srMaxX = 150, srMaxZ = 200;

    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, pavementTex); 
    setupRepeatedTexture();

    glColor3f(0.9f, 0.9f, 0.9f); 
    float tS = 0.02f;
    float bW = 30.0f, fD = 80.0f;
    glBegin(GL_QUADS); 
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); 
    glVertex3f(srMinX - bW, 0.02f, srMaxZ); 
    glTexCoord2f(10, 0); 
    glVertex3f(srMaxX + bW, 0.02f, srMaxZ);
    glTexCoord2f(10, 2); 
    glVertex3f(srMaxX + bW, 0.02f, srMaxZ + fD); 
    glTexCoord2f(0, 2); 
    glVertex3f(srMinX - bW, 0.02f, srMaxZ + fD);

    glTexCoord2f(0, 0); 
    glVertex3f(srMaxX, 0.02f, -200 - bW); 
    glTexCoord2f(1, 0); 
    glVertex3f(srMaxX + bW, 0.02f, -200 - bW);
    glTexCoord2f(1, 10);
    glVertex3f(srMaxX + bW, 0.02f, srMaxZ); 
    glTexCoord2f(0, 10); 
    glVertex3f(srMaxX, 0.02f, srMaxZ);
    glTexCoord2f(0, 0); 
    glVertex3f(srMinX - bW, 0.02f, -200 - bW); 
    glTexCoord2f(1, 0);
    glVertex3f(srMinX, 0.02f, -200 - bW);
    glTexCoord2f(1, 10);
    glVertex3f(srMinX, 0.02f, srMaxZ);
    glTexCoord2f(0, 10); 
    glVertex3f(srMinX - bW, 0.02f, srMaxZ);
    glTexCoord2f(0, 0);
    glVertex3f(srMinX - bW, 0.02f, -200 - bW);
    glTexCoord2f(10, 0); 
    glVertex3f(srMaxX + bW, 0.02f, -200 - bW);
    glTexCoord2f(10, 1); 
    glVertex3f(srMaxX + bW, 0.02f, -200); 
    glTexCoord2f(0, 1); 
    glVertex3f(srMinX - bW, 0.02f, -200);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    structures.drawGrandOpeningDecor(200, 280);
    vegetation.drawFlowerBed(60, 0.02f, 240); 
    vegetation.drawFlowerBed(-60, 0.02f, 240);

    vegetation.drawTree(srMaxX + 15, 0.02f, srMaxZ + 15);
    vegetation.drawTree(srMinX - 15, 0.02f, srMaxZ + 15);
}

void CityMasterPlan::buildMainBoulevard(bool isNight) {
    float startZ = 280.0f;
    float entryWidth = 100.0f; 
    float entryLength = 100.0f;
    float splitZ = startZ + entryLength;

    roadSys.drawRoadSegment(0, 0.05f, startZ, entryWidth, entryLength, false);
    roadSys.drawSidewalk(entryWidth / 2 + 15, 0.0f, startZ + entryLength / 2, 30, entryLength, 0.5f);
    roadSys.drawSidewalk(-(entryWidth / 2 + 15), 0.0f, startZ + entryLength / 2, 30, entryLength, 0.5f);

    vegetation.drawTree(entryWidth / 2 + 10, 0.5f, startZ + 50);
    vegetation.drawTree(-(entryWidth / 2 + 10), 0.5f, startZ + 50);

    for (int i = 0; i < 3; i++) {
        furniture.drawGardenLamp(entryWidth / 2 + 5, 0.5f, startZ + 20 + i * 40, isNight);
        furniture.drawGardenLamp(-(entryWidth / 2 + 5), 0.5f, startZ + 20 + i * 40, isNight);
    }

    float roundaboutCenterZ = 600.0f;
    float roadLen = roundaboutCenterZ - splitZ;
    float laneWidth = 40.0f;
    float medianWidth = 20.0f;

    roadSys.drawRoadSegment(medianWidth / 2 + laneWidth / 2, 0.05f, splitZ, laneWidth, roadLen - 80, false);
    roadSys.drawRoadSegment(-(medianWidth / 2 + laneWidth / 2), 0.05f, splitZ, laneWidth, roadLen - 80, false);
    roadSys.drawSidewalk(0, 0.0f, splitZ + (roadLen - 80) / 2, medianWidth, roadLen - 80, 0.5f);

    for (int i = 0; i < 5; i++) 
        furniture.drawGardenLamp(0, 0.5f, splitZ + 20 + i * 40, isNight);

    roadSys.drawSidewalk(medianWidth / 2 + laneWidth + 10, 0.0f, splitZ + (roadLen - 80) / 2, 20, roadLen - 80, 0.5f);
    roadSys.drawSidewalk(-(medianWidth / 2 + laneWidth + 10), 0.0f, splitZ + (roadLen - 80) / 2, 20, roadLen - 80, 0.5f);

    float northStart = roundaboutCenterZ + (80 + 40);
    float northLength = 1500.0f;
    roadSys.drawRoadSegment(medianWidth / 2 + laneWidth / 2, 0.05f, northStart, laneWidth, 1000.0f, false);
    roadSys.drawRoadSegment(-(medianWidth / 2 + laneWidth / 2), 0.05f, northStart, laneWidth, 1000.0f, false);
    roadSys.drawSidewalk(0, 0.0f, northStart + 500, medianWidth, 1000.0f, 0.5f);

    for (int i = 0; i < 7; i++)
        vegetation.drawTree(0, 0.5f, northStart + 50 + i * 150);

    float lampX = medianWidth / 2 + laneWidth + 10;
    drawStreetLightRow(lampX, northStart + 40, 80.0f, 18, true, isNight);
    drawStreetLightRow(-lampX, northStart + 40, 80.0f, 18, false, isNight);

    roadSys.drawCrosswalk(medianWidth / 2 + laneWidth / 2, 0.05f, startZ + 10, laneWidth, 15.0f, false);
    roadSys.drawCrosswalk(-(medianWidth / 2 + laneWidth / 2), 0.05f, startZ + 10, laneWidth, 15.0f, false);
    roadSys.drawCrosswalk(medianWidth / 2 + laneWidth / 2, 0.05f, 550.0f, laneWidth, 15.0f, false);
    roadSys.drawCrosswalk(-(medianWidth / 2 + laneWidth / 2), 0.05f, 550.0f, laneWidth, 15.0f, false);
}

void CityMasterPlan::buildParkingZone(bool isNight) {
    float pX = 350.0f;
    float pZ = 600.0f;
    float pW = 300.0f; 
    float pL = 400.0f; 
    float border = 40.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pavementTex); 
    setupRepeatedTexture();

    glColor3f(0.9f, 0.9f, 0.9f);
    float tS = 0.02f; 
    float totalW = pW + border * 2;
    float totalL = pL + border * 2;

    glBegin(GL_QUADS); 
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); 
    glVertex3f(pX - totalW / 2, 0.02f, pZ - totalL / 2);
    glTexCoord2f(totalW * tS, 0);
    glVertex3f(pX + totalW / 2, 0.02f, pZ - totalL / 2);
    glTexCoord2f(totalW * tS, totalL * tS);
    glVertex3f(pX + totalW / 2, 0.02f, pZ + totalL / 2);
    glTexCoord2f(0, totalL * tS);
    glVertex3f(pX - totalW / 2, 0.02f, pZ + totalL / 2);
    glEnd(); 
    glDisable(GL_TEXTURE_2D);

    float entryW = 80.0f;
    roadSys.drawSidewalk((150.0f + (pX - pW / 2)) / 2, 0.0f, pZ, (pX - pW / 2) - 150.0f, entryW, 0.025f);

    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, parkingTex);
    setupRepeatedTexture();
    glColor3f(0.85f, 0.85f, 0.85f);
    tS = 1.0f / 40.0f;

    glBegin(GL_QUADS); 
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0);
    glVertex3f(pX - pW / 2, 0.15f, pZ - pL / 2);
    glTexCoord2f(pW * tS, 0); 
    glVertex3f(pX + pW / 2, 0.15f, pZ - pL / 2);
    glTexCoord2f(pW * tS, pL * tS);
    glVertex3f(pX + pW / 2, 0.15f, pZ + pL / 2);
    glTexCoord2f(0, pL * tS); 
    glVertex3f(pX - pW / 2, 0.15f, pZ + pL / 2);
    glEnd(); 
    glDisable(GL_TEXTURE_2D);

    int numRows = 3; 
    float rowHeight = pL / numRows;
    for (int i = 0; i < numRows; i++) {
        float cZ = (pZ - pL / 2) + rowHeight * i + rowHeight / 2;
        if (i < numRows - 1) {
            roadSys.drawSidewalk(pX, 0.0f, cZ + rowHeight / 2, pW - 20, 8.0f, 0.20f);
            vegetation.drawTree(pX, 0.20f, cZ + rowHeight / 2);
            furniture.drawGardenLamp(pX - 50, 0.20f, cZ + rowHeight / 2, isNight);
            furniture.drawGardenLamp(pX + 50, 0.20f, cZ + rowHeight / 2, isNight);
        }
        roadSys.drawParkingLines(pX, cZ, pW - 40, rowHeight - 20, 1, 12);
        furniture.drawParkingCanopy(pX, 0.10f, cZ - (rowHeight - 20) / 2 + 2, pW - 40, 5.0f);

        int carsPerRow = 7;
        float spotWidth = (pW - 40) / carsPerRow;
        for (int k = 0; k < carsPerRow; k++) {
            int randSeed = (int)(i * 99 + k * 17);
            if (randSeed % 5 == 0) 
                continue;
            float carX = (pX - (pW - 40) / 2) + k * spotWidth + spotWidth / 2;
            createAndDrawRandomCar(carX, 0.15f, cZ - (rowHeight - 20) / 2 + 15.0f, 90.0f, randSeed);
            if ((randSeed + 1) % 3 != 0) {
                createAndDrawRandomCar(carX, 0.15f, cZ + (rowHeight - 20) / 2 - 15.0f, -90.0f, randSeed + 7);
            }
        }
    }

    float perimX = pW / 2 + border / 2; float perimZ = pL / 2 + border / 2;
    for (int i = 0; i < 3; i++) {
        vegetation.drawTree(pX + perimX, 0.02f, pZ - 100 + i * 100);
        furniture.drawGardenLamp(pX + perimX, 0.02f, pZ - 100 + i * 100 + 30, isNight);
        vegetation.drawTree(pX - perimX, 0.02f, pZ - 100 + i * 100);
    }
    vegetation.drawTree(pX, 0.02f, pZ + perimZ);
}

void CityMasterPlan::buildCommercialZone() {
    float cX = -350.0f; float cZ = 200.0f; float cW = 300.0f; float cL = 400.0f;
    roadSys.drawSidewalk(cX, 0.0f, cZ, cW, cL, 0.2f);
    for (int row = 0; row < 2; row++) for (int col = 0; col < 2; col++) {
        float tx = cX - cW / 4 + col * (cW / 2);
        float tz = cZ - cL / 4 + row * (cL / 2);
        furniture.drawCafeSet(tx, 0.2f, tz);
    }
    structures.drawKiosk(cX - cW / 2 + 30, 0.2f, cZ + cL / 2 - 30);
    structures.drawKiosk(cX + cW / 2 - 30, 0.2f, cZ + cL / 2 - 30);
    vegetation.drawFlowerBed(cX, 0.2f, cZ);
}
