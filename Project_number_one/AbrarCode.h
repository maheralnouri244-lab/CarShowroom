#ifndef ABRARCODE_H
#define ABRARCODE_H

#include <GL/glut.h>
#include "BeetleCar.h"
#include <vector>

// ????? ???????
enum InteractionState { STATE_WALKING, STATE_ENTERING, STATE_DRIVING };

// ???? ?????? ??????? ??????
struct CarData {
    float x, y, z, scale, rotY;
    int colorIdx;

    // ??????? ??????? ??????? [FrontLeft, FrontRight, RearLeft, RearRight]
    float currentDoors[4];
    float targetDoors[4];
};

class AbrarCode {
public:
    AbrarCode();

    void drawCars();
    void update(float& camX, float& camY, float& camZ, float& yaw, float& pitch);
    void handleInput(unsigned char key, float camX, float camZ);

    InteractionState getState() { return currentState; }

private:
    BeetleCar myCar;
    std::vector<CarData> cars;
    InteractionState currentState;
    int activeCarIndex;

    // ??????? ????????? ????????
    float animT;
    float startX, startY, startZ, startYaw, startPitch;
    float targetX, targetY, targetZ, targetYaw, targetPitch;

    // ???? ?????? ??????
    void attemptEnterCar(float camX, float camZ);
    void driveActiveCar(float speed, float turn);
    float dist(float x1, float z1, float x2, float z2);
};

#endif
