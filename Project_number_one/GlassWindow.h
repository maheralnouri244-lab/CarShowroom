#ifndef GLASSWINDOW_H
#define GLASSWINDOW_H
#include <GL/glut.h>

class GlassWindow {
public:
    GlassWindow(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3,
        float x4, float y4, float z4,
        float a = 0.7f);

    void setAlpha(float a);
    void setTexture(GLuint id);
    void setColor(float r, float g, float b);
    void draw() const;

private:
    float v[4][3];
    float color[3];
    float alpha;
    GLuint texID;
    bool useTexture;
};

#endif