#include "GlassWindow.h"

GlassWindow::GlassWindow(float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float x4, float y4, float z4,
    float a) {
    v[0][0] = x1; v[0][1] = y1; v[0][2] = z1;
    v[1][0] = x2; v[1][1] = y2; v[1][2] = z2;
    v[2][0] = x3; v[2][1] = y3; v[2][2] = z3;
    v[3][0] = x4; v[3][1] = y4; v[3][2] = z4;
    alpha = a;
    texID = 0;
    useTexture = false;
    color[0] = 0.9f;  color[1] = 0.8f;  color[2] = 0.5f;
    color[0] = 0.8f;  color[1] = 0.6f;  color[2] = 0.2f;
    color[0] = 1.0f;  color[1] = 0.9f;  color[2] = 0.6f;
    color[0] = 0.95f; color[1] = 0.85f; color[2] = 0.45f;
    color[0] = 0.6f; color[1] = 0.8f; color[2] = 1.0f;
}

void GlassWindow::setAlpha(float a) { alpha = a; }

void GlassWindow::setTexture(GLuint id) {
    texID = id;
    useTexture = true;
}

void GlassWindow::setColor(float r, float g, float b) {
    color[0] = r; color[1] = g; color[2] = b;
    useTexture = false;
}

void GlassWindow::draw() const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (useTexture && texID) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID);
        glColor4f(1.0f, 1.0f, 1.0f, alpha);
    }
    else {
        glDisable(GL_TEXTURE_2D);
        glColor4f(color[0], color[1], color[2], alpha);
    }

    glBegin(GL_QUADS);
    if (useTexture && texID) {
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(v[0]);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(v[1]);
        glTexCoord2f(1.0f, 1.0f); glVertex3fv(v[2]);
        glTexCoord2f(0.0f, 1.0f); glVertex3fv(v[3]);
    }
    else {
        glVertex3fv(v[0]);
        glVertex3fv(v[1]);
        glVertex3fv(v[2]);
        glVertex3fv(v[3]);
    }
    glEnd();

    if (useTexture && texID) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }

    glDisable(GL_BLEND);
}