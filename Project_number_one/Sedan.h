#include "Car.h"

class Sedan : public Car {
public:
    Sedan(float x, float y, float z, float rot, float r, float g, float b) : Car(x, y, z, rot, r, g, b, 2.0f) {}
    void draw() override;
};