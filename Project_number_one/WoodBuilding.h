#ifndef WOODBUILDING_H
#define WOODBUILDING_H

#include <GL/glut.h>
#include <random>
#include <vector>

class WoodBuilding {
private:
    std::mt19937 rng;
public:
    int minRows = 10, maxRows = 20;
    int minCols = 3, maxCols = 7;
    int minPillarH = 0, maxPillarH = 4;
    int minWallWood = 3, maxWallWood = 100;
    int minRoofWood = 3, maxRoofWood = 100;
    int minDoorOffset = 0, maxDoorOffset = 3;
    std::vector<int> indices, indices2, indices3, indices4, indices5;
    int chosenFront, REP;
    int chosenBack;
    int chosenLeft;
    int chosenRight;
    int chosenRoof;
    int rows, cols;
    int pillarH;
    int doorOffset;

    int randInt(int min, int max);
    void drawWoodCube(GLuint textureID, float length, float height, float depth);
    void drawRandomWoodLine(GLuint textureID, int total, int chosen,
        float startX, float startY, float startZ,
        float stepX, float stepY, float stepZ,
        float woodL, float woodH, float woodD, std::vector<int>& indices);
    WoodBuilding();
    void generate();
    void drawHouse(GLuint textureID);
};

#endif