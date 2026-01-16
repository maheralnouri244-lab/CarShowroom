#ifndef MAHERHEADER_H
#define MAHERHEADER_H
#include <GL/glut.h>
#include "Showroomdoor.h"
#include "lake.h"

class Maherheader {
public:
    Maherheader();
    void draw(float camX, float camY, float camZ);
    void init();
    unsigned int loadTextureFromFile(const char* path);
private:
    unsigned int houseTexID[5];
    ShowroomDoor showroomdoor;
};

#endif