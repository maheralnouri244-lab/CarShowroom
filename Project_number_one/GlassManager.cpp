#include "GlassManager.h"
#include <GL/glut.h>

GlassManager::GlassManager() {}

void GlassManager::addWindow(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float x4, float y4, float z4) {
    windows.push_back(GlassWindow(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, 0.5f));
}

void GlassManager::initShowroomWindows() {
    float h = 60.0f;
    float maxX = 150.0f, maxz = 200.0f, diff = 45.0f;
    float minX = -maxX, minz = -maxz;

    addWindow(maxX, 0, minz, maxX, h, minz, maxX, h, maxz, maxX, 0, maxz);
    addWindow(minX, 0, minz, minX, h, minz, minX, h, maxz, minX, 0, maxz);
    addWindow(maxX, 0, minz, maxX, h, minz, minX, h, minz, minX, 0, minz);
    addWindow(minX, 0, maxz, minX, h, maxz, minX + (maxX - minX - diff) / 2 + 3, h, maxz, minX + (maxX - minX - diff) / 2 + 3, 0, maxz);
    addWindow(maxX, 0, maxz, maxX, h, maxz, maxX - (maxX - minX - diff) / 2 - 3, h, maxz, maxX - (maxX - minX - diff) / 2 - 3, 0, maxz);

    addWindow(minX + (maxX - minX - diff) / 2, 0, (maxz - minz - 2 * diff) / 2, minX + (maxX - minX - diff) / 2, h, (maxz - minz - 2 * diff) / 2, minX + (maxX - minX - diff) / 2, h, -(maxz - minz - 2 * diff) / 2, minX + (maxX - minX - diff) / 2, 0, -(maxz - minz - 2 * diff) / 2);
    addWindow(maxX - (maxX - minX - diff) / 2, 0, (maxz - minz - 2 * diff) / 2, maxX - (maxX - minX - diff) / 2, h, (maxz - minz - 2 * diff) / 2, maxX - (maxX - minX - diff) / 2, h, -(maxz - minz - 2 * diff) / 2, maxX - (maxX - minX - diff) / 2, 0, -(maxz - minz - 2 * diff) / 2);

    float floorY = 60.0f;
    float h2 = h + floorY;

    addWindow(maxX, floorY, minz, maxX, h2, minz, maxX, h2, maxz, maxX, floorY, maxz);
    addWindow(minX, floorY, minz, minX, h2, minz, minX, h2, maxz, minX, floorY, maxz);
    addWindow(maxX, floorY, minz, maxX, h2, minz, minX, h2, minz, minX, floorY, minz);
/*    addWindow(minX, floorY, maxz, minX, h2, maxz, minX + (maxX - minX - diff) / 2 + 3, h2, maxz, minX + (maxX - minX - diff) / 2 + 3, floorY, maxz);
    addWindow(maxX, floorY, maxz, maxX, h2, maxz, maxX - (maxX - minX - diff) / 2 - 3, h2, maxz, maxX - (maxX - minX - diff) / 2 - 3, floorY, maxz);*/
    addWindow(minX, floorY, maxz, minX, h2, maxz, maxX, h2, maxz, maxX, floorY, maxz);
}

void GlassManager::drawAll() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_LIGHTING);
    glDepthMask(GL_TRUE);

    for (const auto& win : windows) {
        win.draw();
    }

    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}