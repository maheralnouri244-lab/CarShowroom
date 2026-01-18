#include "AbrarCode.h"
#include <math.h>
#include <stdio.h>
#include <vector>

// --- تضمين ملفات البناء ---
#include "Pillar.h"
#include "Hpillar.h"
#include "Showroomside.h"
#include "GlassWindow.h"
#include "NeonTube.h"
// -------------------------

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// إحداثيات المصعد
const float ELEV_X = -120.0f;
const float ELEV_Z = -170.0f;
const float FLOOR_H = 60.0f;

AbrarCode::AbrarCode() {
    currentState = STATE_WALKING;
    activeCarIndex = -1;
    animT = 0.0f;

    // Elevator Init (Starts at Ground Floor 0.0)
    elevatorY = -60.0f;
    targetElevatorY = -60.0f;
    elevState = ELEV_IDLE;
    doorGapGround = 0.0f;
    doorGapSecond = 0.0f;
    doorGapCabin = 0.0f;
    playerInElevator = false;
    elevatorDoorsOpen = true; // Initially open at ground
    elevatorDoorOffset = 10.0f; // Visual only

    float pX = 120.0f, s = 8.0f, r = -90.0f;
    cars.push_back({ pX, 2.0f, -40.0f, s, r, 0, {0,0,0,0}, {0,0,0,0} });
    cars.push_back({ pX, 2.0f, -75.0f, s, r, 1, {0,0,0,0}, {0,0,0,0} });
    cars.push_back({ pX, 2.0f, -110.0f, s, r, 2, {0,0,0,0}, {0,0,0,0} });
    cars.push_back({ pX, 2.0f, -145.0f, s, r, 3, {0,0,0,0}, {0,0,0,0} });
}

void AbrarCode::drawCars() {
    for (int i = 0;i < cars.size();i++) {
        bool drv = (currentState == STATE_DRIVING && i == activeCarIndex);
        myCar.draw(cars[i].x, cars[i].y, cars[i].z, cars[i].scale, cars[i].rotY, cars[i].colorIdx, drv, cars[i].currentDoors, cars[i].currentSteer);
    } 
}
float AbrarCode::dist(float x1, float z1, float x2, float z2) { 
    return sqrt(pow(x2 - x1, 2) + pow(z2 - z1, 2)); 
}
void AbrarCode::attemptEnterCar(float cx, float cz) { 
    int cl = -1;float md = 50;
    for (int i = 0;i < cars.size();i++) { 
        float d = dist(cx, cz, cars[i].x, cars[i].z);
        if (d < md) { 
            md = d;cl = i; 
        } 
    } if (cl != -1) { 
        activeCarIndex = cl;
        currentState = STATE_ENTERING;
        animT = 0;cars[activeCarIndex].targetDoors[1] = 60; 
    }
}

void AbrarCode::driveActiveCar(float speed, float turn) {
    if (activeCarIndex == -1)
        return;
    cars[activeCarIndex].rotY -= turn;
    float steerLimit = 45.0f; // أقصى دوران للمقود
    if (turn != 0) {
        cars[activeCarIndex].currentSteer += turn * 5.0f;
        // تقييد الزاوية
        if (cars[activeCarIndex].currentSteer > steerLimit) cars[activeCarIndex].currentSteer = steerLimit;
        if (cars[activeCarIndex].currentSteer < -steerLimit) cars[activeCarIndex].currentSteer = -steerLimit;
    }
    
    float r = cars[activeCarIndex].rotY * M_PI / 180;
    cars[activeCarIndex].x += sin(r) * speed;
    cars[activeCarIndex].z += cos(r) * speed; 
}

// --- INPUT LOGIC ---
void AbrarCode::handleInput(unsigned char key, float& camX, float& camY, float& camZ) {
    if (currentState == STATE_WALKING) {
        // --- Elevator Entry (N) ---
        if (key == 'n' || key == 'N') {
            float d = dist(camX, camZ, ELEV_X, ELEV_Z);

            // Check Player Floor (Ground < 100, Upper >= 100)
            bool playerAtGround = (camY < 100.0f);

            // Check Elevator Position (-60 = Ground, 0 = Upper)
            bool elevAtGround = (fabs(elevatorY - (-60.0f)) < 5.0f);
            bool elevAtUpper = (fabs(elevatorY - 0.0f) < 5.0f);

            if (d < 50.0f) {
                // Scenario 1: Elevator is here -> Enter
                if ((playerAtGround && elevAtGround) || (!playerAtGround && elevAtUpper)) {
                    camX = ELEV_X;
                    camZ = ELEV_Z;
                    // Camera Height Calculation:
                    // If at Ground (-60): 140 + (-60) = 80
                    // If at Upper (0): 140 + 0 = 140
                    camY = 140.0f + elevatorY;

                    currentState = STATE_ELEVATOR;
                    playerInElevator = true;
                    elevState = ELEV_DOOR_CLOSING;
                }
                // Scenario 2: Elevator is away -> Call it
                else {
                    if (playerAtGround) targetElevatorY = -60.0f; // Call Down
                    else targetElevatorY = 0.0f;                  // Call Up

                    if (targetElevatorY > elevatorY) elevState = ELEV_MOVING_UP;
                    else elevState = ELEV_MOVING_DOWN;
                }
            }
        }

        // --- Car Interaction ---
        if (key == 'g' || key == 'G') attemptEnterCar(camX, camZ);

        if (key == 'f' || key == 'F') {
            int c = -1; float md = 40.0f;
            for (int i = 0; i < cars.size(); i++) { float d = dist(camX, camZ, cars[i].x, cars[i].z); if (d < md) { md = d; c = i; } }
            if (c != -1) {
                if (cars[c].targetDoors[1] < 1) cars[c].targetDoors[1] = 60; else cars[c].targetDoors[1] = 0;
            }
        }
    }

    else if (currentState == STATE_ELEVATOR) {
        // --- Elevator Control (M) ---
        if (key == 'm' || key == 'M') {
            if (elevState == ELEV_IDLE || elevState == ELEV_DOOR_OPENING) {
                // Toggle Floor
                if (fabs(elevatorY - 0.0f) < 5.0f) targetElevatorY = -60.0f; // Go Down
                else targetElevatorY = 0.0f;                                 // Go Up

                elevState = ELEV_DOOR_CLOSING;
            }
        }

        // --- Exit Elevator (N) ---
        if (key == 'n' || key == 'N') {
            if (elevState == ELEV_IDLE) {
                elevState = ELEV_DOOR_OPENING;
                playerInElevator = false; // Flag to exit in update()
            }
        }
    }

    else if (currentState == STATE_DRIVING) {
        // --- Car Driving ---
        float s = 2.0f, t = 3.0f;
        switch (key) {
        case 'w': driveActiveCar(s, 0); break;
        case 's': driveActiveCar(-s, 0); break;
        case 'a': driveActiveCar(0, -t); break;
        case 'd': driveActiveCar(0, t); break;
        case 'g':
            cars[activeCarIndex].targetDoors[1] = 0.0f;
            currentState = STATE_WALKING;
            activeCarIndex = -1;
            camY = 80.0f;
            break;
        case 'f':
            if (cars[activeCarIndex].targetDoors[1] < 1) 
                cars[activeCarIndex].targetDoors[1] = 60;
            else 
                cars[activeCarIndex].targetDoors[1] = 0;
            break;
        }
    }
}

// --- UPDATE LOGIC ---
void AbrarCode::update(float& camX, float& camY, float& camZ, float& yaw, float& pitch) {
    float liftSpeed = 0.5f;
    float doorSpeed = 0.2f;

    // --- Elevator Movement ---
    if (elevState == ELEV_MOVING_UP) {
        elevatorY += liftSpeed;
        if (elevatorY >= targetElevatorY) { 
            elevatorY = targetElevatorY; 
            elevState = ELEV_DOOR_OPENING; 
        }
    }
    if (elevState == ELEV_MOVING_DOWN) {
        elevatorY -= liftSpeed;
        if (elevatorY <= targetElevatorY) { 
            elevatorY = targetElevatorY; 
            elevState = ELEV_DOOR_OPENING; 
        }
    }
    // Auto-center steering if not turning
    if (activeCarIndex != -1) {
        // تخفيف الزاوية تدريجياً نحو الصفر
        cars[activeCarIndex].currentSteer *= 0.9f;
        if (fabs(cars[activeCarIndex].currentSteer) < 0.1f) cars[activeCarIndex].currentSteer = 0;
    }
    // --- Door Opening ---
    if (elevState == ELEV_DOOR_OPENING) {
        doorGapCabin += doorSpeed;
        // Ground door at -60 (less than -30), Upper door at 0
        if (elevatorY < -30) 
            doorGapGround += doorSpeed; 
        else 
            doorGapSecond += doorSpeed;

        if (doorGapCabin >= 9.0f) {
            doorGapCabin = 9.0f;
            if (elevatorY < -30) 
                doorGapGround = 9.0f; 
            else 
                doorGapSecond = 9.0f;
            elevState = ELEV_IDLE;

            // Execute Exit Logic
            if (!playerInElevator && currentState == STATE_ELEVATOR) {
                currentState = STATE_WALKING;
                camZ += 40.0f; // Step out
                // Set correct floor height
                if (elevatorY > -30) 
                    camY = 140.0f; // Upper Floor
                else 
                    camY = 80.0f;                  // Ground Floor
            }
        }
    }

    // --- Door Closing ---
    if (elevState == ELEV_DOOR_CLOSING) {
        doorGapCabin -= doorSpeed;
        if (elevatorY < -30) 
            doorGapGround -= doorSpeed; 
        else 
            doorGapSecond -= doorSpeed;

        if (doorGapCabin <= 0.0f) {
            doorGapCabin = 0.0f;
            if (elevatorY < -30) 
                doorGapGround = 0.0f; 
            else 
                doorGapSecond = 0.0f;

            // Check if we need to move after closing
            if (fabs(elevatorY - targetElevatorY) > 1.0f)
                elevState = (targetElevatorY > elevatorY) ? ELEV_MOVING_UP : ELEV_MOVING_DOWN;
            else
                elevState = ELEV_IDLE;
        }
    }

    // --- Camera Sync in Elevator ---
    if (playerInElevator && currentState == STATE_ELEVATOR) {
        camX = ELEV_X;
        camZ = ELEV_Z;
        camY = 140.0f + elevatorY; // 140 + (-60) = 80, 140 + 0 = 140
    }

    // --- Car Animation ---
    myCar.updateAnimation();
    for (int c = 0;c < cars.size();c++) 
        for (int d = 0;d < 4;d++) {
            float& cr = cars[c].currentDoors[d]; 
            float tg = cars[c].targetDoors[d];
            if (cr < tg) 
                cr += 2; 
            else if (cr > tg) 
                cr -= 2;
        }

    // --- Car Entering Animation ---
    if (currentState == STATE_ENTERING) {
        if (animT == 0) {
            startX = camX;
            startY = camY;
            startZ = camZ;
            startYaw = yaw;
            startPitch = pitch;

            CarData& c = cars[activeCarIndex]; 
            float r = c.rotY * M_PI / 180;
            targetX = c.x + (0.4f * c.scale * cos(r) + 0.4f * c.scale * sin(r));
            targetY = c.y + 1.1f * c.scale;
            targetZ = c.z + (-0.4f * c.scale * sin(r) + 0.4f * c.scale * cos(r));
            targetYaw = c.rotY - 90; targetPitch = 0;
        }
        animT += 0.05f; 
        if (animT >= 1) { 
            animT = 1;
            currentState = STATE_DRIVING; 
        }
        camX = startX + (targetX - startX) * animT;
        camY = startY + (targetY - startY) * animT;
        camZ = startZ + (targetZ - startZ) * animT;
        yaw = startYaw + (targetYaw - startYaw) * animT; 
        pitch = startPitch + (targetPitch - startPitch) * animT;
    }
    else if (currentState == STATE_DRIVING) {
        CarData& c = cars[activeCarIndex]; float r = c.rotY * M_PI / 180;
        camX = c.x + (0.4f * c.scale * cos(r) + 0.4f * c.scale * sin(r));
        camZ = c.z + (-0.4f * c.scale * sin(r) + 0.4f * c.scale * cos(r));
        camY = c.y + 1.1f * c.scale;
    }
}

// --- DRAWING ---

void AbrarCode::drawElevatorShaft(float x, float z, float h) {
    float w = 30.0f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glDepthMask(GL_FALSE);
    glColor4f(0.5, 0.7, 0.9, 0.3); 
    glPushMatrix(); 
    glTranslatef(x, h / 2, z); 
    glScalef(w, h, w); 
    glutSolidCube(1); 
    glPopMatrix();
    glDepthMask(GL_TRUE); 
    glDisable(GL_BLEND);
    glColor3f(0.2, 0.2, 0.2); 
    float p[] = { -1,1 }; 
    for (int i = 0;i < 2;i++)
        for (int j = 0;j < 2;j++) { 
            glPushMatrix();
            glTranslatef(x + p[i] * w / 2, h / 2, z + p[j] * w / 2);
            glScalef(2, h, 2);glutSolidCube(1);glPopMatrix(); 
        }
}

void AbrarCode::drawElevatorDoor(float x, float y, float z, float gap) {
    float w = 18, h = 20; 
    glColor3f(0.5, 0.5, 0.55);
    glPushMatrix(); 
    glTranslatef(x - w / 4 - gap / 2, y + h / 2, z); 
    glScalef(w / 2, h, 1); 
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(x + w / 4 + gap / 2, y + h / 2, z); 
    glScalef(w / 2, h, 1); 
    glutSolidCube(1); 
    glPopMatrix();
    glColor3f(0.2, 0.2, 0.2); 

    glPushMatrix(); 
    glTranslatef(x, y + h + 1, z); 
    glScalef(w + 4, 2, 2); 
    glutSolidCube(1); 
    glPopMatrix();
}
void AbrarCode::drawElevatorCabin(float x, float y, float z) {
    float w = 20; 
    glPushMatrix(); 
    glTranslatef(x, y + 10, z);
    glColor3f(0.8, 0.8, 0.85);

    glPushMatrix(); 
    glTranslatef(0, -9.5, 0); 
    glScalef(w, 1, w); 
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(0, 9.5, 0); 
    glScalef(w, 1, w); 
    glutSolidCube(1); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(0, 0, -w / 2); 
    glScalef(w, 20, 1); 
    glutSolidCube(1); 
    glPopMatrix();

    glColor3f(0.5, 0.5, 0.5); 
    drawElevatorDoor(0, -10, w / 2 - 1, doorGapCabin);

    glColor3f(0.1, 0.1, 0.1); 
    glPushMatrix(); 
    glTranslatef(w / 2 - 2, 0, 0); 
    glScalef(1, 6, 2); 
    glutSolidCube(1); 
    glPopMatrix();

    if (elevState == ELEV_MOVING_UP) 
        glColor3f(0, 1, 0); 
    else 
        glColor3f(0, 0.3, 0); 

    glPushMatrix(); 
    glTranslatef(w / 2 - 1.5, 1, 0); 
    glutSolidSphere(0.3, 8, 8); 
    glPopMatrix();

    if (elevState == ELEV_MOVING_DOWN) 
        glColor3f(1, 0, 0);
    else 
        glColor3f(0.3, 0, 0); 
    glPushMatrix(); 
    glTranslatef(w / 2 - 1.5, -1, 0); 
    glutSolidSphere(0.3, 8, 8); 
    glPopMatrix();

    glPopMatrix();
}

void AbrarCode::drawProduct(int type) {
    if (type == 0) { 
        glColor3f(0.9, 0.9, 0.95); 
        glPushMatrix(); 
        glRotatef(90, 1, 0, 0); 
        glutSolidTorus(0.5, 1.8, 10, 20); 
        glPopMatrix(); 
        for (int i = 0;i < 5;i++) { 
            glPushMatrix();
            glRotatef(72 * i, 0, 0, 1);
            glScalef(0.3, 1.5, 0.1);
            glutSolidCube(1);
            glPopMatrix(); 
        } 
    }

    else if (type == 1) { 
        glColor3f(0.15, 0.15, 0.15); 
        glutSolidTorus(0.8, 2.0, 12, 20); 
        glColor3f(0.1, 0.1, 0.1); 
        glutWireTorus(0.81, 2.0, 8, 12); 
    }
    else if (type == 2) { 
        glColor3f(1, 0.8, 0); 
        glPushMatrix(); 
        glScalef(1.5, 2.5, 1); 
        glutSolidCube(1); 
        glPopMatrix(); 
    }
    else if (type == 3) { 
        glColor3f(0.1, 0.1, 0.1); 
        glPushMatrix(); 
        glScalef(2, 1.5, 1.5); 
        glutSolidCube(1); 
        glPopMatrix(); 
        glColor3f(1, 0, 0);

        glPushMatrix(); 
        glTranslatef(0.5, 0.6, 0); 
        glutSolidCube(0.2); 
        glPopMatrix(); 
        glColor3f(0, 0, 1);

        glPushMatrix(); 
        glTranslatef(-0.5, 0.6, 0);
        glutSolidCube(0.2); 
        glPopMatrix(); 
    }
    else { 
        glColor3f(0.8, 0.8, 0.85); 
        GLUquadric* q = gluNewQuadric(); 
        glPushMatrix(); 
        glRotatef(-90, 1, 0, 0); 
        gluCylinder(q, 0.8, 0.8, 2, 12, 1); 
        glPopMatrix(); 
        gluDeleteQuadric(q); 
    }
}

void AbrarCode::drawMassiveShelf(float x, float y, float z, int rows, int cols, int type) {
    glPushMatrix(); 
    glTranslatef(x, y, z);
    float w = cols * 6.0f, h = rows * 6.0f, d = 4.0f;
    glColor3f(0.4, 0.25, 0.1); 
    glPushMatrix(); glTranslatef(0, h / 2, -d / 2); glScalef(w, h, 0.5); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(-w / 2, h / 2, 0); glScalef(0.5, h, d); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(w / 2, h / 2, 0); glScalef(0.5, h, d); glutSolidCube(1); glPopMatrix();
    for (int r = 0;r < rows;r++) {
        glColor3f(0.5, 0.3, 0.15); glPushMatrix(); glTranslatef(0, r * 6 + 2, 0); glScalef(w, 0.4, d); glutSolidCube(1); glPopMatrix();
        for (int c = 0;c < cols;c++) { glPushMatrix(); glTranslatef(-w / 2 + 3 + c * 6, r * 6 + 3, 0); drawProduct(type); glPopMatrix(); }
    }
    glPopMatrix();
}

void AbrarCode::drawCheckoutCounter(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z);
    glColor3f(0.9, 0.9, 0.9); glPushMatrix(); glTranslatef(0, 5, 0); glScalef(40, 10, 8); glutSolidCube(1); glPopMatrix();
    glColor3f(0.1, 0.1, 0.1); glPushMatrix(); glTranslatef(5, 11, 0); glScalef(6, 4, 1); glutSolidCube(1); glPopMatrix();
    glColor3f(0.8, 0.8, 0.8); glPushMatrix(); glTranslatef(-10, 10.5, 0); glScalef(4, 1, 4); glutSolidCube(1); glPopMatrix();
    SteveModel s(0.3f); 
    SteveModel::setShirtColor(1, 1, 1); 
    glPushMatrix(); 
    glTranslatef(0, 0, -6); 
    glRotatef(180, 0, 1, 0); 
    s.draw(); 
    glPopMatrix();
    glPopMatrix();
}

void AbrarCode::drawQueue(float x, float y, float z) {
    SteveModel c(0.3f); for (int i = 0;i < 6;i++) { SteveModel::setShirtColor(0.1 * (i + 1), 0.5, 0.8 - 0.1 * i); glPushMatrix(); glTranslatef(x, y, z + 20 + i * 15); glRotatef(180, 0, 1, 0); c.draw(); glPopMatrix(); }
}

void AbrarCode::drawVideoWall(float x, float y, float z, float w, float h) {
    glPushMatrix(); glTranslatef(x, y, z);
    glColor3f(0.1, 0.1, 0.1); glPushMatrix(); glScalef(w + 2, h + 2, 1); glutSolidCube(1); glPopMatrix();
    float t = glutGet(GLUT_ELAPSED_TIME) / 500.0; glColor3f(fabs(sin(t)), fabs(cos(t * 0.5)), 0.8);
    glPushMatrix(); glTranslatef(0, 0, 0.6); glScalef(w, h, 0.1); glutSolidCube(1); glPopMatrix();
    glPopMatrix();
}

void AbrarCode::drawSecretariatOffice(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.7, 0.8, 1, 0.3); glPushMatrix(); glTranslatef(-30, 20, 0); glScalef(2, 40, 100); glutSolidCube(1); glPopMatrix();
    glDisable(GL_BLEND);
    glColor3f(0.4, 0.2, 0.1); glPushMatrix(); glTranslatef(10, 5, -20); glScalef(40, 10, 10); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(25, 5, 0); glScalef(10, 10, 50); glutSolidCube(1); glPopMatrix();
    SteveModel s(0.3f); SteveModel::setShirtColor(0.9, 0.6, 0.6); glPushMatrix(); glTranslatef(15, 0, -30); glRotatef(150, 0, 1, 0); s.draw(); glPopMatrix();
    furniture.drawTableSet(0, 0, 30);
    glColor3f(0.5, 0.5, 0.5); glPushMatrix(); glTranslatef(40, 15, -40); glScalef(15, 30, 5); glutSolidCube(1); glPopMatrix();
    glPopMatrix();
}

void AbrarCode::drawLuxurySofa(float x, float y, float z, float r) { glPushMatrix(); glTranslatef(x, y, z); glRotatef(r, 0, 1, 0); glColor3f(0.3, 0.1, 0.05); glPushMatrix(); glScalef(20, 4, 8); glutSolidCube(1); glPopMatrix(); glPushMatrix(); glTranslatef(0, 6, -3); glScalef(20, 8, 2); glutSolidCube(1); glPopMatrix(); glPushMatrix(); glTranslatef(9, 3, 0); glScalef(2, 6, 8); glutSolidCube(1); glPopMatrix(); glPushMatrix(); glTranslatef(-9, 3, 0); glScalef(2, 6, 8); glutSolidCube(1); glPopMatrix(); glPopMatrix(); }
void AbrarCode::drawChandelier(float x, float y, float z) { glPushMatrix(); glTranslatef(x, y, z); glColor3f(1, 0.9, 0); glPushMatrix(); glTranslatef(0, 5, 0); glScalef(0.5, 10, 0.5); glutSolidCube(1); glPopMatrix(); for (int i = 0;i < 3;i++) { glPushMatrix();glRotatef(30 * i, 0, 1, 0);glutWireTorus(0.2, 5 - i, 10, 20);glPopMatrix(); } glPopMatrix(); }
void AbrarCode::drawHologram(float x, float y, float z) { glPushMatrix(); glTranslatef(x, y, z); glColor3f(0.1, 0.1, 0.1); glPushMatrix(); glScalef(15, 2, 15); glutSolidCube(1); glPopMatrix(); glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE); glColor4f(0, 0.8, 1, 0.5); float s = glutGet(GLUT_ELAPSED_TIME) * 0.05; glRotatef(s, 0, 1, 0); glTranslatef(0, 6, 0); glutWireSphere(4, 20, 20); glScalef(0.7, 0.7, 0.7); glColor4f(1, 1, 1, 0.8); glutSolidSphere(3, 20, 20); glDisable(GL_BLEND); glPopMatrix(); }
void AbrarCode::drawFountain(float x, float y, float z) { glPushMatrix(); glTranslatef(x, y, z); glColor3f(0.8, 0.8, 0.9); glPushMatrix(); glRotatef(-90, 1, 0, 0); glutSolidCone(8, 3, 20, 2); glPopMatrix(); glColor3f(0.2, 0.5, 1); glPushMatrix(); glTranslatef(0, 4, 0); glScalef(1, 6, 1); glutSolidCube(1); glPopMatrix(); glPopMatrix(); }
void AbrarCode::drawTV(float x, float y, float z, float r) { glPushMatrix(); glTranslatef(x, y, z); glRotatef(r, 0, 1, 0); glColor3f(0.1, 0.1, 0.1); glPushMatrix(); glScalef(20, 12, 1); glutSolidCube(1); glPopMatrix(); float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0; glColor3f(fabs(sin(t)), fabs(cos(t)), 0.5); glPushMatrix(); glTranslatef(0, 0, 0.6); glScalef(18, 10, 0.1); glutSolidCube(1); glPopMatrix(); glPopMatrix(); }
void AbrarCode::drawTechStation(float x, float y, float z) { glPushMatrix(); glTranslatef(x, y, z); glColor3f(0.3, 0.3, 0.35); glPushMatrix(); glTranslatef(0, 4, 0); glScalef(10, 8, 10); glutSolidCube(1.0); glPopMatrix(); glColor3f(0.6, 0.6, 0.6); glPushMatrix(); glTranslatef(0, 10, 0); glutSolidTeapot(3.0); glPopMatrix(); glPopMatrix(); }
void AbrarCode::drawInfoKiosk(float x, float y, float z) { glPushMatrix(); glTranslatef(x, y, z); glColor3f(0.1, 0.1, 0.1); glPushMatrix(); glTranslatef(0, 8, 0); glScalef(4, 16, 2); glutSolidCube(1.0); glPopMatrix(); glColor3f(0.0, 0.5, 1.0); glPushMatrix(); glTranslatef(0, 12, 1.1); glScalef(3, 4, 0.1); glutSolidCube(1.0); glPopMatrix(); glPopMatrix(); }
void AbrarCode::drawPeopleOnFloor(float fy) { 
    SteveModel s(0.3f); 
    float p[10][3] = { 
        {80,fy,-50},
        {-80,fy,50},
        {0,fy,100},
        {50,fy,150},
        {-50,fy,-100},
        {100,fy,0},
        {-100,fy,0},
        {20,fy,20},
        {-20,fy,-20},
        {0,fy,-150} 
    }; 
    for (int i = 0;i < 10;i++) { 
        SteveModel::setShirtColor(0.1 * (i + 1), 0.5, 0.8); 
        glPushMatrix(); 
        glTranslatef(p[i][0], fy + 0.2, p[i][2]); 
        glRotatef(i * 45, 0, 1, 0); s.draw(); glPopMatrix(); 
    } }

void AbrarCode::drawMegaAccessoriesShop(float x, float y, float z) {
    for (int i = 0; i < 5; i++) drawMassiveShelf(-120.0f, 0, -100.0f + (i * 50.0f), 5, 6, i % 5);
    float backZ = -180.0f;
    drawCheckoutCounter(50.0f, 0, backZ);
    drawVideoWall(50.0f, 30.0f, backZ + 2.0f, 60.0f, 30.0f);
    drawQueue(50.0f, 0, backZ + 25.0f);
}

void AbrarCode::drawGroundFloorElevator() {
    drawElevatorShaft(ELEV_X, ELEV_Z, 120.0f);
    drawElevatorDoor(ELEV_X, 0, ELEV_Z + 15, doorGapGround);
}

void AbrarCode::drawSecondFloor() {
    float floorY = FLOOR_H;
    float h = 60.0f;
    float maxX = 150.0f, maxZ = 200.0f, diff = 45.0f;
    float minX = -maxX, minZ = -maxZ;

    glPushMatrix();
    glTranslatef(0.0f, floorY, 0.0f);

    // 1. OPAQUE (الأرضية والسقف والهيكل)
    glColor3f(0.85f, 0.85f, 0.9f); glBegin(GL_QUADS); glNormal3f(0, 1, 0); glVertex3f(maxX, 0.1f, maxZ); glVertex3f(maxX, 0.1f, minZ); glVertex3f(minX, 0.1f, minZ); glVertex3f(minX, 0.1f, maxZ); glEnd();
    glColor3f(1, 1, 1); glBegin(GL_QUADS); glNormal3f(0, -1, 0); glVertex3f(maxX, h, maxZ); glVertex3f(maxX, h, minZ); glVertex3f(minX, h, minZ); glVertex3f(minX, h, maxZ); glEnd();

    // الأعمدة والجدران (نفس الكود السابق تماماً)
    std::vector<Pillar> pillars; pillars.push_back(Pillar(maxX, 0, maxZ, h)); for (const auto& p : pillars) p.draw();
    std::vector<Hpillar> hpillars; hpillars.push_back(Hpillar(maxX, 0, 0, 0.4, 0.8, maxZ - minZ)); hpillars.push_back(Hpillar(maxX, h - 0.4, 0, 0.4, 0.8, maxZ - minZ));
    hpillars.push_back(Hpillar(minX, 0, 0, 0.4, 0.8, maxZ - minZ)); hpillars.push_back(Hpillar(minX, h - 0.4, 0, 0.4, 0.8, maxZ - minZ));
    hpillars.push_back(Hpillar(0, 0, minZ, 0.4, maxX - minX, 0.8)); hpillars.push_back(Hpillar(0, h - 0.4, minZ, 0.4, maxX - minX, 0.8));
    hpillars.push_back(Hpillar(0, h - 0.4, maxZ, 0.4, maxX - minX, 0.8)); hpillars.push_back(Hpillar(minX + (maxX - minX - diff) / 4, 0, maxZ, 0.4, (maxX - minX - diff) / 2, 0.8)); hpillars.push_back(Hpillar(maxX - (maxX - minX - diff) / 4, 0, maxZ, 0.4, (maxX - minX - diff) / 2, 0.8));
    for (const auto& p : hpillars) p.draw();

    std::vector<Showroomside> showroomsides; showroomsides.push_back(Showroomside(maxX, minX, minZ, minZ, h, 0.2, 0.2, 0.2, 40, 5)); showroomsides.push_back(Showroomside(minX, minX, maxZ, minZ, h, 0.2, 0.2, 0.2, 40, 5)); showroomsides.push_back(Showroomside(maxX, maxX, maxZ, minZ, h, 0.2, 0.2, 0.2, 40, 5)); showroomsides.push_back(Showroomside(minX + (maxX - minX - diff) / 2, minX, maxZ, maxZ, h, 0.2, 0.2, 0.2, 18, 5)); showroomsides.push_back(Showroomside(maxX, maxX - (maxX - minX - diff) / 2, maxZ, maxZ, h, 0.2, 0.2, 0.2, 18, 5)); for (const auto& p : showroomsides) p.draw();
    std::vector<NeonTube> neonyubes; neonyubes.push_back(NeonTube(minX + (maxX - minX - diff) / 2 + 4, 0, 0, maxZ - minZ - diff, 0.1, 'z')); neonyubes.push_back(NeonTube(maxX - (maxX - minX - diff) / 2 - 4, 0, 0, maxZ - minZ - diff, 0.1, 'z')); for (const auto& p : neonyubes) p.draw();

    // المحتويات
    float rW = 60, rH = 40;
    drawRoom(-100, 0, 0, rW, rH, 300);
    drawRoom(100, 0, -150, rW, rH, 80);
    drawRoom(100, 0, 150, rW, rH, 80);
    drawMegaAccessoriesShop(-100, 0, 0);
    drawSecretariatOffice(120, 0, 50);
    drawLuxurySofa(100, 2, 120, 45); drawChandelier(100, 30, 120); drawFountain(120, 0, 150);
    drawHologram(100, 2, 150);
    drawPeopleOnFloor(0.2f);

    glPopMatrix();

    // الكابينة (في الإحداثيات العالمية)
    drawElevatorCabin(ELEV_X, elevatorY, ELEV_Z);

    // 2. GLASS (الآن نرسمه تماماً كما في الأرضي، بدون أي تغييرات خاصة)
    glPushMatrix(); glTranslatef(0, floorY, 0);
    std::vector<GlassWindow> glasswindows;
    glasswindows.push_back(GlassWindow(maxX, 0, minZ, maxX, h, minZ, minX, h, minZ, minX, 0, minZ));
    glasswindows.push_back(GlassWindow(maxX, 0, minZ, maxX, h, minZ, maxX, h, maxZ, maxX, 0, maxZ));
    glasswindows.push_back(GlassWindow(minX, 0, minZ, minX, h, minZ, minX, h, maxZ, minX, 0, maxZ));
    glasswindows.push_back(GlassWindow(minX, 0, maxZ, minX, h, maxZ, minX + (maxX - minX - diff) / 2 + 3, h, maxZ, minX + (maxX - minX - diff) / 2 + 3, 0, maxZ));
    glasswindows.push_back(GlassWindow(maxX, 0, maxZ, maxX, h, maxZ, maxX - (maxX - minX - diff) / 2 - 3, h, maxZ, maxX - (maxX - minX - diff) / 2 - 3, 0, maxZ));

    // إزالة أي إعدادات دمج يدوية هنا، وترك GlassWindow::draw يتعامل معها
    // فقط نتأكد من تفعيل الدمج العام في main
    for (const auto& p : glasswindows) p.draw();

    drawElevatorDoor(ELEV_X, 0, ELEV_Z + 15, doorGapSecond);

    glPopMatrix();
}


void AbrarCode::drawRoom(float x, float y, float z, float w, float h, float d) {
    Pillar p1(x - w / 2, y, z - d / 2, h); p1.draw(); Pillar p2(x + w / 2, y, z - d / 2, h); p2.draw();
    Pillar p3(x + w / 2, y, z + d / 2, h); p3.draw(); Pillar p4(x - w / 2, y, z + d / 2, h); p4.draw();
    NeonTube n(x, y + h - 1, z, d - 2, 0.2, 'z'); n.draw();
}
