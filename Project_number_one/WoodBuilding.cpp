#include "WoodBuilding.h"

WoodBuilding::WoodBuilding() {
    rng.seed(std::random_device{}());
    rows = cols = pillarH = doorOffset = 0;
}

int WoodBuilding::randInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

void WoodBuilding::generate() {
    rows = randInt(minRows, maxRows);
    cols = randInt(minCols, maxCols);

    pillarH = randInt(minPillarH, maxPillarH)*3;
    doorOffset = randInt(minDoorOffset, maxDoorOffset);
    indices.assign(9*cols, 0);
    for (int i = 0; i < 9 * cols; ++i) indices[i] = i;
    std::shuffle(indices.begin(), indices.end(), rng);
    indices2.assign(9 * cols, 0);
    for (int i = 0; i < 9 * cols; ++i) indices2[i] = i;
    std::shuffle(indices2.begin(), indices2.end(), rng);
    indices3.assign(rows, 0);
    for (int i = 0; i < rows; ++i) indices3[i] = i;
    std::shuffle(indices3.begin(), indices3.end(), rng);
    indices4.assign(rows, 0);
    for (int i = 0; i < rows; ++i) indices4[i] = i;
    std::shuffle(indices4.begin(), indices4.end(), rng);
    indices5.assign(rows, 0);
    for (int i = 0; i < rows; ++i) indices5[i] = i;
    std::shuffle(indices5.begin(), indices5.end(), rng);

    REP = randInt(1, 2);
    chosenFront = randInt(minWallWood, 9 * cols);
    chosenBack = randInt(minWallWood, 9 * cols);
    chosenLeft = randInt(minWallWood, rows);
    chosenRight = randInt(minWallWood, rows);
    //chosenRoof = randInt(minRoofWood, rows);
}

void WoodBuilding::drawWoodCube(unsigned int textureID, float length, float height, float depth) {
    float lx = length / 2.0f;
    float hy = height / 2.0f;
    float dz = depth / 2.0f;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-lx, -hy, dz);
    glTexCoord2f(1, 0); glVertex3f(lx, -hy, dz);
    glTexCoord2f(1, 1); glVertex3f(lx, hy, dz);
    glTexCoord2f(0, 1); glVertex3f(-lx, hy, dz);
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(-lx, -hy, -dz);
    glTexCoord2f(1, 0); glVertex3f(lx, -hy, -dz);
    glTexCoord2f(1, 1); glVertex3f(lx, hy, -dz);
    glTexCoord2f(0, 1); glVertex3f(-lx, hy, -dz);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-lx, -hy, -dz);
    glTexCoord2f(1, 0); glVertex3f(-lx, -hy, dz);
    glTexCoord2f(1, 1); glVertex3f(-lx, hy, dz);
    glTexCoord2f(0, 1); glVertex3f(-lx, hy, -dz);
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(lx, -hy, -dz);
    glTexCoord2f(1, 0); glVertex3f(lx, -hy, dz);
    glTexCoord2f(1, 1); glVertex3f(lx, hy, dz);
    glTexCoord2f(0, 1); glVertex3f(lx, hy, -dz);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-lx, hy, dz);
    glTexCoord2f(1, 0); glVertex3f(lx, hy, dz);
    glTexCoord2f(1, 1); glVertex3f(lx, hy, -dz);
    glTexCoord2f(0, 1); glVertex3f(-lx, hy, -dz);
    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-lx, -hy, dz);
    glTexCoord2f(1, 0); glVertex3f(lx, -hy, dz);
    glTexCoord2f(1, 1); glVertex3f(lx, -hy, -dz);
    glTexCoord2f(0, 1); glVertex3f(-lx, -hy, -dz);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void WoodBuilding::drawRandomWoodLine(unsigned int textureID, int total, int chosen,
    float startX, float startY, float startZ,
    float stepX, float stepY, float stepZ,
    float woodL, float woodH, float woodD,std::vector<int> &indices) {
    for (int rep=0;rep<REP;rep++)
    for (int i = 0; i < chosen; ++i) {
        int idx = indices[i];
        float x = startX + idx * stepX;
        float y = startY + rep * stepY;
        float z = startZ + idx * stepZ;
        glPushMatrix();
        glTranslatef(x, y, z);
        drawWoodCube(textureID, woodL, woodH, woodD);
        glPopMatrix();
    }
}

void WoodBuilding::drawHouse(unsigned int textureID) {
    float woodL = 20.0f, woodH = 1, woodD = 2;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            glPushMatrix();
            glTranslatef(c * woodL, pillarH, r * woodD);
            drawWoodCube(textureID, woodL, woodH, woodD);
            glPopMatrix();
        }
    }

    glPushMatrix();
    glTranslatef(1, pillarH / 2.0f,1);
    drawWoodCube(textureID, 1, pillarH, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((cols-1) * woodL- 1, pillarH / 2.0f, 1);
    drawWoodCube(textureID, 1, pillarH, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((cols-1) * woodL - 1, pillarH / 2.0f, rows*woodD-1);
    drawWoodCube(textureID, 1, pillarH, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1, pillarH / 2.0f, rows * woodD - 1);
    drawWoodCube(textureID, 1, pillarH, 1);
    glPopMatrix();

    int totalFront = rows;
    drawRandomWoodLine(textureID, totalFront, chosenFront,
        0, pillarH + woodL/2, rows * woodD-0.5,
        woodD, woodL, 0,
        woodD, woodL, woodH,indices);

    int totalBack = rows;
    drawRandomWoodLine(textureID, totalBack, chosenBack,
        0, pillarH + woodL / 2, 0,
        woodD, woodL, 0,
        woodD, woodL, woodH, indices2);

    int totalLeft = rows;
    drawRandomWoodLine(textureID, totalLeft, chosenLeft,
        -woodL/2+0.5, pillarH + woodL / 2, 0,
        0, woodL, woodD,
        woodH, woodL, woodD, indices3);
    int totalRight = rows;
    drawRandomWoodLine(textureID, totalRight, chosenRight,
        cols*woodL -woodL/2- 0.5, pillarH + woodL / 2, 0,
        0, woodL, woodD,
        woodH, woodL, woodD, indices4);
    
    /*
    int totalRoof = cols;
    drawRandomWoodLine(textureID, totalRoof, chosenRoof,
        0, pillarH + rows * woodH + 1.0f, rows * woodD / 2.0f,
        woodL, 0, 0,
        woodL, woodH, woodD,indices5);
        */
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            glPushMatrix();
            glTranslatef(c * woodL, pillarH+REP*woodL, r * woodD);
            drawWoodCube(textureID, woodL, woodH, woodD);
            glPopMatrix();
        }
    }
}