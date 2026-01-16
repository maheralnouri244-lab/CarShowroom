#pragma once
#include <GL/glut.h>

class ShowroomDoor {
private:
    float x, y, z;       
    float width, height; 
    float offsetY;       

public:
    ShowroomDoor(float posX, float posY, float posZ,
        float w = 45.0f, float h = 30.0f);

    void draw() const;
    void moveUp(float step);  
    void moveDown(float step);
};