#include "CameraSphere.h"
#include <cmath>

CameraSphere::CameraSphere(float cubeSize, float sphereRadius, float smallCubeSize)
    : cubeSize(cubeSize), sphereRadius(sphereRadius), smallCubeSize(smallCubeSize),
    lookX(1.0f), lookY(0.0f), lookZ(0.0f) {}

void CameraSphere::setLookDirection(float lx, float ly, float lz) {
    lookX = lx;
    lookY = ly;
    lookZ = lz;
}

void CameraSphere::draw() {
    glPushMatrix();

    glColor3f(0.3, 0.3, 0.3);
    glutSolidCube(cubeSize);

    glTranslatef(cubeSize / 2, cubeSize / 2, cubeSize / 2);
    glColor3f(0.8, 0.8, 0.8);
    glutSolidSphere(sphereRadius, 32, 32);

    float norm = sqrt(lookX * lookX + lookY * lookY + lookZ * lookZ);
    float nx = lookX / norm;
    float ny = lookY / norm;
    float nz = lookZ / norm;

    glTranslatef(nx * sphereRadius, ny * sphereRadius, nz * sphereRadius);
    glColor3f(0.1, 0.1, 0.1);
    glutSolidCube(smallCubeSize);

    glPopMatrix();
}