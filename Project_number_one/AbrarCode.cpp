#include "AbrarCode.h"
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

AbrarCode::AbrarCode() {
    currentState = STATE_WALKING;
    activeCarIndex = -1;
    animT = 0.0f;
    float pX = 90.0f, s = 8.0f, r = -90.0f;
    // الباب رقم 1 هو السائق (حسب تجربتنا السابقة)
    cars.push_back({ pX, 2.0f, -40.0f, s, r, 0, {0,0,0,0}, {0,0,0,0} });
    cars.push_back({ pX, 2.0f, -75.0f, s, r, 1, {0,0,0,0}, {0,0,0,0} });
    cars.push_back({ pX, 2.0f, -110.0f, s, r, 2, {0,0,0,0}, {0,0,0,0} });
    cars.push_back({ pX, 2.0f, -145.0f, s, r, 3, {0,0,0,0}, {0,0,0,0} });
}

void AbrarCode::drawCars() {
    for (int i = 0; i < cars.size(); i++) {
        bool drv = (currentState == STATE_DRIVING && i == activeCarIndex);
        myCar.draw(cars[i].x, cars[i].y, cars[i].z, cars[i].scale, cars[i].rotY, cars[i].colorIdx, drv, cars[i].currentDoors);
    }
}

float AbrarCode::dist(float x1, float z1, float x2, float z2) {
    return sqrt(pow(x2 - x1, 2) + pow(z2 - z1, 2));
}

void AbrarCode::attemptEnterCar(float camX, float camZ) {
    int closest = -1;
    float minD = 50.0f;
    for (int i = 0; i < cars.size(); i++) {
        float d = dist(camX, camZ, cars[i].x, cars[i].z);
        if (d < minD) { minD = d; closest = i; }
    }
    if (closest != -1) {
        activeCarIndex = closest;
        currentState = STATE_ENTERING;
        animT = 0.0f;
        // فتح باب السائق (رقم 1)
        cars[activeCarIndex].targetDoors[1] = 60.0f;
    }
}

void AbrarCode::driveActiveCar(float speed, float turn) {
    if (activeCarIndex == -1) return;
    cars[activeCarIndex].rotY -= turn;
    float rad = cars[activeCarIndex].rotY * M_PI / 180.0f;
    // الحركة: +sin/+cos
    cars[activeCarIndex].x += sin(rad) * speed;
    cars[activeCarIndex].z += cos(rad) * speed;
}

void AbrarCode::handleInput(unsigned char key, float camX, float camZ) {
    if (currentState == STATE_WALKING) {
        if (key == 'g' || key == 'G') attemptEnterCar(camX, camZ);

        if (key == 'f' || key == 'F') {
            int closestCar = -1;
            float minD = 40.0f;
            for (int i = 0; i < cars.size(); i++) {
                float d = dist(camX, camZ, cars[i].x, cars[i].z);
                if (d < minD) { minD = d; closestCar = i; }
            }

            if (closestCar != -1) {
                // التحكم بباب السائق فقط (رقم 1)
                int driverDoorIdx = 1;
                if (cars[closestCar].targetDoors[driverDoorIdx] < 1.0f)
                    cars[closestCar].targetDoors[driverDoorIdx] = 60.0f;
                else
                    cars[closestCar].targetDoors[driverDoorIdx] = 0.0f;
            }
        }
    }
    else if (currentState == STATE_DRIVING) {
        float s = 2.0f, t = 3.0f;
        switch (key) {
        case 'w': driveActiveCar(s, 0); break;
        case 's': driveActiveCar(-s, 0); break;
        case 'a': driveActiveCar(0, -t); break;
        case 'd': driveActiveCar(0, t); break;
        case 'g':
            // إغلاق الباب عند الخروج
            cars[activeCarIndex].targetDoors[1] = 0.0f;
            currentState = STATE_WALKING;
            activeCarIndex = -1;
            break;
        case 'f':
            // التحكم بالباب من الداخل
            if (cars[activeCarIndex].targetDoors[1] < 1.0f) cars[activeCarIndex].targetDoors[1] = 60.0f;
            else cars[activeCarIndex].targetDoors[1] = 0.0f;
            break;
        }
    }
}

void AbrarCode::update(float& camX, float& camY, float& camZ, float& yaw, float& pitch) {
    // Animate Doors
    for (int c = 0; c < cars.size(); c++) {
        for (int d = 0; d < 4; d++) {
            float& curr = cars[c].currentDoors[d];
            float targ = cars[c].targetDoors[d];
            if (curr < targ) { curr += 2.0f; if (curr > targ) curr = targ; }
            else if (curr > targ) { curr -= 2.0f; if (curr < targ) curr = targ; }
        }
    }

    if (currentState == STATE_ENTERING) {
        if (animT == 0.0f) {
            startX = camX; startY = camY; startZ = camZ; startYaw = yaw; startPitch = pitch;
            CarData& c = cars[activeCarIndex];
            float rad = c.rotY * M_PI / 180.0f;
            float lx = 0.4f * c.scale;
            // رفعنا الكاميرا قليلاً
            float ly = 1.1f * c.scale;
            float lz = 0.4f * c.scale;
            targetX = c.x + (lx * cos(rad) + lz * sin(rad));
            targetY = c.y + ly;
            targetZ = c.z + (-lx * sin(rad) + lz * cos(rad));
            targetYaw = c.rotY - 90.0f;
            targetPitch = 0.0f;
        }
        animT += 0.05f;
        if (animT >= 1.0f) { animT = 1.0f; currentState = STATE_DRIVING; }
        camX = startX + (targetX - startX) * animT;
        camY = startY + (targetY - startY) * animT;
        camZ = startZ + (targetZ - startZ) * animT;

        // أثناء الدخول فقط نجبر النظر للأمام
        yaw = startYaw + (targetYaw - startYaw) * animT;
        pitch = startPitch + (targetPitch - startPitch) * animT;
    }
    else if (currentState == STATE_DRIVING) {
        CarData& c = cars[activeCarIndex];
        float rad = c.rotY * M_PI / 180.0f;
        float lx = 0.4f * c.scale;
        float ly = 1.1f * c.scale;
        float lz = 0.4f * c.scale;

        // تحديث الموقع فقط (اترك yaw و pitch للماوس)
        camX = c.x + (lx * cos(rad) + lz * sin(rad));
        camZ = c.z + (-lx * sin(rad) + lz * cos(rad));
        camY = c.y + ly;
    }
}
