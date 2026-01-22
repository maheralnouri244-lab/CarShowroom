#include "Car.h"

class SUV : public Car {
public:
    SUV(float x, float y, float z, float rot, float r, float g, float b) : Car(x, y, z, rot, r, g, b, 2.2f) {}
    void draw() override;
};