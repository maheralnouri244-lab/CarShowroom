#ifndef GLASSMANAGER_H
#define GLASSMANAGER_H

#include <vector>
#include "GlassWindow.h"

class GlassManager {
public:
    GlassManager();

    void addWindow(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3,
        float x4, float y4, float z4);

    void drawAll();
    void initShowroomWindows();

private:
    std::vector<GlassWindow> windows;
};

#endif
