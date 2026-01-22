#include "Car.h"

class Taxi : public Car {
public:
    Taxi(float x, float y, float z, float rot) : Car(x, y, z, rot, 1.0f, 0.8f, 0.0f, 2.0f) {}
    void draw() override;
};