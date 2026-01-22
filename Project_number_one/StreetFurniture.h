#include <GL/glut.h>

#include "City.h"

class StreetFurniture : public City{
public:
    void drawBench(float x, float y, float z, float rotation);
    void drawGardenLamp(float x, float y, float z, bool isOn);
    void drawStreetLampBase(float x, float y, float z);
    void drawParkingCanopy(float x, float y, float z, float width, float length);
    void drawCafeSet(float x, float y, float z);
};