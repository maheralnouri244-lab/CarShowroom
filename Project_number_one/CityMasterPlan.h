#ifndef CITYMASTERPLAN_H
#define CITYMASTERPLAN_H

#include "StreetLamp.h"
#include "Car.h"
#include "Garden.h"
#include "Roadway.h"
#include "Vegetation.h"
#include "StreetFurniture.h"
#include "Structures.h"

class CityMasterPlan {
public:
    CityMasterPlan();

    void setTextures(unsigned int road, unsigned int pavement, unsigned int grass, unsigned int parkingAsphalt, unsigned int water, unsigned int wall, unsigned int treeBark, unsigned int treeLeaf);

    void drawCityLayout(bool isNight);

    void drawGardenWater() { myGarden.drawWaterSurface(-280.0f, 0.0f, 600.0f); }
    void drawGardenReflection(void (*scene)()) { myGarden.drawWaterReflection(-280.0f, 0.0f, 600.0f, scene); }

private:
    Roadway roadSys;
    Vegetation vegetation;
    StreetFurniture furniture;
    Structures structures;
    Garden myGarden;
    StreetLamp streetLightObj;

    unsigned int roadTex, pavementTex, grassTex, parkingTex, waterTex, wallTex;

    void createAndDrawRandomCar(float x, float y, float z, float rotation, int seed);
    void drawStreetLightRow(float x, float startZ, float spacing, int count, bool isRightSide, bool isNight);

    void buildShowroomPlaza();
    void buildMainBoulevard(bool isNight);
    void buildParkingZone(bool isNight);
    void buildCommercialZone();
    void drawLakeParkArea(bool isNight);
};

#endif