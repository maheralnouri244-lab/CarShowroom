#include "PersistentCloud.h"
#include <cstdlib>
#include <ctime>

PersistentCloud::PersistentCloud() {
    regenerate();
}

void PersistentCloud::regenerate() {
    balls.clear();

    posX = (rand() % 2000 - 1000);
    posY = (rand() % 200 + 500);
    posZ = (rand() % 2000 - 1000);
    cloudScale = (rand() % 10 + 10.0f);

    int numBalls = (rand() % 15 + 10);
    for (int i = 0; i < numBalls; ++i) {
        CloudBall b;
        b.x = (rand() % 100 - 50) / 15.0f;
        b.y = (rand() % 50 - 25) / 20.0f;
        b.z = (rand() % 100 - 50) / 15.0f;
        b.radius = (rand() % 100 / 100.0f) * 1.2f + 0.6f;
        b.grayShade = 0.7f + (rand() % 20 / 100.0f);
        balls.push_back(b);
    }
}

void PersistentCloud::draw(int st) const {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glScalef(cloudScale, cloudScale, cloudScale);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_LIGHTING);

    for (const auto& b : balls) {
        glPushMatrix();
        glTranslatef(b.x, b.y, b.z);
        if (st == 2)
            glScalef(2, 2, 2);
        glColor4f(b.grayShade, b.grayShade, b.grayShade, 0.4f);
        glutSolidSphere(b.radius, 12, 12);
        glPopMatrix();
    }

    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glPopMatrix();
}