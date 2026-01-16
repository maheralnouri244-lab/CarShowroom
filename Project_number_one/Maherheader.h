#ifndef MAHERHEADER_H
#define MAHERHEADER_H
#include <GL/glut.h>

class Maherheader {
public:
    Maherheader();
    void draw() const;
    void init();
    unsigned int loadTextureFromFile(const char* path);
private:
    unsigned int houseTexID[5];
};

#endif