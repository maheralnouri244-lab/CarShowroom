#include <GL/glut.h>
#include <vector>
#include "City.h"

class Roadway : public City{
public:
    void setTextures(unsigned int road, unsigned int pavement, unsigned int parking, unsigned int grass);

    void drawInfiniteGround();
    void drawRoadSegment(float x, float y, float z, float width, float length, bool isHorizontal);
    void drawSidewalk(float x, float y, float z, float width, float length, float height);
    void drawCurvedSidewalk(float x, float y, float z, float radius, float startAngle, float endAngle, float width);
    void drawCrosswalk(float x, float y, float z, float width, float length, bool isHorizontal);
    void drawParkingLines(float x, float z, float width, float length, int rows, int cols);

private:
    unsigned int roadTex, pavementTex, parkingTex, grassTex;
    void drawRoadMarkings(float x, float y, float z, float width, float length, bool isHorizontal);
};