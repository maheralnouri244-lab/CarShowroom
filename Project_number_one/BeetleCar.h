#ifndef BEETLECAR_H
#define BEETLECAR_H
#include <GL/glut.h>

class BeetleCar {
public:
    BeetleCar();
    void draw(float x, float y, float z, float scale, float rotateY, int colorIndex = 0);
    void toggleDoors();
    void updateAnimation();

private:
    float currentDoorAngle;
    float targetDoorAngle;
    const float* currentBodyColor;

    void setMaterial(const float* color, float shine = 0.0f, float alphaOverride = -1.0f);
    void drawDisk(float radius, int slices);
    void drawCylinder(float radius, float height, int slices);

    void drawWhitewallWheel();
    void drawSeat();
    void drawGenericDoor(bool isRight, bool hasMirror);
    void drawExtendedChassis();
    void drawOpenCabin();
    void drawDetails();
    void drawFrontEnd();
    void drawGlass();
};
#endif
