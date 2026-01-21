#ifndef F1CAR_H
#define F1CAR_H

#include <GL/glut.h>
#include <cmath>

class F1Car {
private:
    float bodyColor[3];

    // Static color constants
    static const float COL_BLACK[];
    static const float COL_GREY[];
    static const float COL_DARK[];

    // Helper to set material color
    void setColor(const float* color);

    // Geometry Helpers
    static void drawCylinder(float radius, float height, int slices);
    static void drawTaperedBox(float w_base, float w_tip, float h, float len);

    // Component Drawers
    void drawWheel(bool isRear);
    void drawChassis();
    void drawNose();
    void drawWing(float w, float d, bool fins);

public:
    // Constructor: Sets the specific color of this car instance
    F1Car(float r, float g, float b);

    // Main Draw Function
    void draw();
};

#endif
