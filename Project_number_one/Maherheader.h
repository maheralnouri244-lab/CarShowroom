#ifndef MAHERHEADER_H
#define MAHERHEADER_H
#include <GL/glut.h>
#include "Showroomdoor.h"
#include "lake.h"

class Maherheader {
public:
    Maherheader();
    void draw(float camX, float camY, float camZ,bool isday);
    void init();
    unsigned int loadTextureFromFile(const char* path);
    void drawLightBeam(float x, float y, float z);
private:
    unsigned int houseTexID[5];
    ShowroomDoor showroomdoor;
};

#endif