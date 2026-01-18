#ifndef MONITOR_SCREEN_H
#define MONITOR_SCREEN_H

#include <GL/glut.h>

class MonitorScreen {
private:
    float width, height, depth;
    float standHeight, standRadius;
    float baseRadius, baseHeight;
    GLfloat bodyColor[4];

public:
    MonitorScreen(float w = 24.0f, float h = 16.0f, float d = 2.5f);
    void draw(GLuint textures[4]);

private:
    void drawBase();
    void drawStand();
    void drawMonitorHead(GLuint textures[4]);
    void drawQuadrant(float x1, float y1, float x2, float y2, GLuint texID, float zPos);
};

#endif