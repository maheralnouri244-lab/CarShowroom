#include "ShowroomGate.h"
#include "StageLight.h"

void ShowroomGate::draw() const {
    float r = 0.05f, g = 0.05f, b = 0.1f;
    float width = 45;
    float height = 60.0f;
    float depth = 1.0f;
    float cx = 0.0f, cy = 15.0f, cz = 200.0f;
    float entryHeight = 30.0f;


    glColor3f(r, g, b);

    glPushMatrix();
    glTranslatef(cx - width / 2 + 2.0f, entryHeight / 2, cz);
    glScalef(2.0f, entryHeight, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx + width / 2 - 2.0f, entryHeight / 2, cz);
    glScalef(2.0f, entryHeight, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx, (entryHeight + (height - entryHeight) / 2), cz);
    glScalef(width, height - entryHeight, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(r + 0.1f, g + 0.1f, b + 0.1f);
    glPushMatrix();
    glTranslatef(cx, height + 1.0f, cz);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.2, width / 2, 16, 32);
    glPopMatrix();
}

void ShowroomGate::drawGateArch(float cx, float cz) {

    StageLight light;
    float r = 0.05f, g = 0.05f, b = 0.1f;
    float width = 45;
    float height = 60.0f;
    float depth = 1.0f;

    float lightRotations[10][4] = {
    {150.0f,0.0f,   0.0f,   0.0f},
    {160.0f,0.0f,  45.0f,   0.0f},
    {170.0f,  0.0f,90.0f,   0.0f},
    {180.0f, 0.0f,135.0f,   0.0f},
    {190.0f, 0.0f,180.0f,   0.0f},
    {200.0f, 0.0f,225.0f,   0.0f},
    {210.0f, 0.0f,270.0f,   0.0f},
    {155.0f, 0.0f,315.0f,   0.0f},
    {165.0f,  0.0f,60.0f,   0.0f},
    {175.0f, 0.0f,120.0f,   0.0f}
    };

    glColor3f(r, g, b);

    // «·⁄„Êœ «·√Ì”—
    glPushMatrix();
    glTranslatef(cx - width / 2 + 1.0f, height / 2, cz);
    glScalef(2.0f, height, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    // «·⁄„Êœ «·√Ì„‰
    glPushMatrix();
    glTranslatef(cx + width / 2 - 1.0f, height / 2, cz);
    glScalef(2.0f, height, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(r + 0.1f, g + 0.1f, b + 0.15f);
    glPushMatrix();
    glTranslatef(cx, height, cz);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(1.0, width / 2, 32, 64);

    float radius = width / 2 + 2.0f; 
    int numLights = 10;

    for (int i = 0; i < numLights; i++) {
        float angle = i * (360.0f / numLights);

        glPushMatrix();
        glRotatef(angle, 0.0f,-1.0f, 180.0f);
        glTranslatef(radius-2, 0.0f, 2);  
        glScalef(0.4f, 0.4f, 0.4f);        
        light.setRotation(lightRotations[i][0],
            lightRotations[i][1],
            lightRotations[i][2]);
        light.draw();
        glPopMatrix();
    }

    glPopMatrix();
}
