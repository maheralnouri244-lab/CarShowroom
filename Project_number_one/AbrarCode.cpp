#include "AbrarCode.h"

AbrarCode::AbrarCode() {}

void AbrarCode::draw4Cars() {
    float parkingX = 120.0f;
    float scale = 8.0f;
    float rot = -90.0f;

    // Car 1 (Blue - 0)
    myCar.draw(parkingX, 2.0f, -40.0f, scale, rot, 0);

    // Car 2 (Red - 1)
    myCar.draw(parkingX, 2.0f, -75.0f, scale, rot, 1);

    // Car 3 (Green - 2)
    myCar.draw(parkingX, 2.0f, -110.0f, scale, rot, 2);

    // Car 4 (Yellow - 3)
    myCar.draw(parkingX, 2.0f, -145.0f, scale, rot, 3);
}


void AbrarCode::interact() { myCar.toggleDoors(); }
void AbrarCode::update() { myCar.updateAnimation(); }
