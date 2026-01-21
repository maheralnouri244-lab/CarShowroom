#ifndef CITYMASTERPLAN_H
#define CITYMASTERPLAN_H

#include <GL/glut.h>
#include <cmath>
#include <vector>
#include "Garden.h"
#include "StreetLamp.h"
#include "Car.h"
#include "Model.h"

class CityMasterPlan {
public:
    CityMasterPlan();
    void setTextures(unsigned int road, unsigned int pavement, unsigned int grass,
        unsigned int parkingAsphalt, unsigned int water, unsigned int wall,
        unsigned int treeBark, unsigned int treeLeaf);
    void drawCityLayout(bool isNight);

    void drawGardenWater() { myGarden.drawWaterSurface(-280.0f, 0.0f, 600.0f); }
    void drawGardenReflection(void (*scene)()) { myGarden.drawWaterReflection(-280.0f, 0.0f, 600.0f, scene); }

    void drawText3D(const char* text, float x, float y, float z, float scale, float r, float g, float b);
    void drawShowroomSign(bool isNight);
    void drawRoadSegment(float x, float y, float z, float width, float length, bool isHorizontal);
    Garden myGarden;
    StreetLamp streetLightObj;

    unsigned int roadTex, pavementTex, grassTex, parkingTex, waterTex, wallTex;

    void drawStreetLightRow(float x, float startZ, float spacing, int count, bool isRightSide, bool isNight);

    void drawRoadMarkings(float x, float y, float z, float width, float length, bool isHorizontal);
    void drawSidewalk(float x, float z, float width, float length, float height);
    void drawCurvedSidewalk(float x, float y, float z, float radius, float startAngle, float endAngle, float width);
    void drawInfiniteGround();
    void drawParkingLines(float x, float z, float width, float length, int rows, int cols);
    void drawCrosswalk(float x, float y, float z, float width, float length, bool isHorizontal);

    void drawRoundabout(float x, float y, float z, float radius, float roadWidth);
    void drawModernBench(float x, float y, float z, float rotation);
    void drawWoodenBridge(float x, float y, float z, float length, float width);
    void drawGazebo(float x, float y, float z);
    void drawFlowerBed(float x, float y, float z);
    void drawSimpleTree(float x, float y, float z);
    void drawParkingCanopy(float x, float y, float z, float width, float length);
    void drawGardenLamp(float x, float y, float z, bool isOn);
    void drawCafeSet(float x, float y, float z);
    void drawModernKiosk(float x, float y, float z);
    void drawStreetLampBase(float x, float y, float z);
    void drawFence(float w, float l);
    void drawMainGate();
    void drawBalloon(float x, float y, float z, float r, float g, float b);
    void drawGrandOpeningDecor();

    void createAndDrawRandomCar(float x, float y, float z, float rotation, int seed);

    void buildShowroomPlaza();
    void drawLakePark(bool isNight);

    void buildMainBoulevard(bool isNight);
    void drawJuiceStall(float x, float y, float z);
    void buildCommercialZone();
    void buildParkingZone();
private:
    Model treeModel;
    unsigned int treeBarkTexID;
    unsigned int treeLeafTexID;
};

#endif