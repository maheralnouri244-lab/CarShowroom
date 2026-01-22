#include "Car.h"

class Ambulance : public Car {
public:
    Ambulance(float x, float y, float z, float rot) : Car(x, y, z, rot, 0.95f, 0.95f, 0.95f, 2.4f) {}
    void draw() override;
};