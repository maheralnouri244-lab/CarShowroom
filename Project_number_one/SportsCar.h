#include "Car.h"

class SportsCar : public Car {
public:
    SportsCar(float x, float y, float z, float rot, float r, float g, float b) : Car(x, y, z, rot, r, g, b, 2.1f) {}
    void draw() override;
};