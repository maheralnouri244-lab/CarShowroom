#ifndef SARACODE_H
#define SARACODE_H

#include "NormalCar.h"
#include <vector>

struct SaraData {
    float x, y, z, rotY;
    int colorIdx;
    float doorAngle;
};

class SaraCode {
public:
    SaraCode();
    void drawAll();
    void update();
    void handleInput(unsigned char key);
private:
    NormalCar model;
    std::vector<SaraData> cars;
};

#endif