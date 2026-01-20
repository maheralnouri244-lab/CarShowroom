#ifndef CAR_H
#define CAR_H

#include <GL/glut.h>
#include <cmath>

class Car {
protected:
    float x, y, z;
    float rotation;
    float r, g, b;
    float scale;

    void drawWheel(float dx, float dy, float dz, float radius, float width);

    void drawChassis(float w, float h, float l, float r, float g, float b);
    void drawCabin(float w, float h, float l, float topRatio, float r, float g, float b);

public:
    Car(float _x, float _y, float _z, float _rot, float _r, float _g, float _b, float _scale = 1.0f);
    virtual ~Car() {}
    virtual void draw() = 0;
};

class Sedan : public Car {
public:
    Sedan(float x, float y, float z, float rot, float r, float g, float b) : Car(x, y, z, rot, r, g, b, 2.0f) {}
    void draw() override;
};

class SUV : public Car {
public:
    SUV(float x, float y, float z, float rot, float r, float g, float b) : Car(x, y, z, rot, r, g, b, 2.2f) {}
    void draw() override;
};

class SportsCar : public Car {
public:
    SportsCar(float x, float y, float z, float rot, float r, float g, float b) : Car(x, y, z, rot, r, g, b, 2.1f) {}
    void draw() override;
};

class Taxi : public Car {
public:
    Taxi(float x, float y, float z, float rot) : Car(x, y, z, rot, 1.0f, 0.8f, 0.0f, 2.0f) {}
    void draw() override;
};

class Ambulance : public Car {
public:
    Ambulance(float x, float y, float z, float rot) : Car(x, y, z, rot, 0.95f, 0.95f, 0.95f, 2.4f) {}
    void draw() override;
};

#endif
