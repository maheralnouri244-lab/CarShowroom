#include "House.h"
#include <cmath>


void ProceduralHouse::draw(unsigned int textureID) const {

    glColor3f(0.8f, 0.8f, 0.8f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

    glVertex3f(-1.0f, -1.0f, -1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);   glVertex3f(1.0f, -1.0f, -1.0f);

    glVertex3f(-1.0f, 1.0f, -1.0f);  glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);    glVertex3f(1.0f, 1.0f, -1.0f);

    glVertex3f(-1.0f, -1.0f, -1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);   glVertex3f(-1.0f, -1.0f, 1.0f);

    glVertex3f(1.0f, -1.0f, -1.0f);  glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);    glVertex3f(1.0f, -1.0f, 1.0f);

    glVertex3f(-1.0f, -1.0f, -1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);   glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    glColor3f(0.7f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 2.5f, 1.0f);
    glVertex3f(0.0f, 2.5f, -1.0f);

    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(0.0f, 2.5f, -1.0f);
    glVertex3f(0.0f, 2.5f, 1.0f);
    glEnd();

    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_TRIANGLES);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 2.5f, 1.0f);

    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(0.0f, 2.5f, -1.0f);
    glEnd();
}