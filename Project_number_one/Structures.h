#include <GL/glut.h>
#include <vector>
#include "City.h"

class Structures : public City{
public:
    void drawGazebo(float x, float y, float z);
    void drawWoodenBridge(float x, float y, float z, float length, float width);
    void drawFence(float w, float l);
    void drawMainGate();
    void drawShowroomSign(bool isNight);
    void drawKiosk(float x, float y, float z);
    void drawJuiceStall(float x, float y, float z);
    void drawGrandOpeningDecor(float startZ, float endZ);

private:
    void drawText3D(const char* text, float x, float y, float z, float scale, float r, float g, float b);
    void drawBalloon(float x, float y, float z, float r, float g, float b);
};