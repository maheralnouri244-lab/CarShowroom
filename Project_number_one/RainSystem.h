#ifndef RAIN_SYSTEM_H
#define RAIN_SYSTEM_H

#include <GL/glut.h>
#include <vector>
#include <cstdlib>

struct RainDrop {
    float x, y, z;
    float speed;
    float length;
};

class RainSystem {
private:
    std::vector<RainDrop> drops;

public:
    RainSystem(int count = 1000) {
        drops.resize(count);

        for (int i = 0; i < (int)drops.size(); i++) {
            resetDrop(i);
            drops[i].y = (float)(rand() % 500);
        }
    }

    void resetDrop(int i) {
        drops[i].x = (float)((rand() % 4000) - 2000);
        drops[i].y = 1400.0f + (rand() % 200);
        drops[i].z = (float)((rand() % 4000) - 2000);
        drops[i].speed = 2.0f + (float)(rand() % 10 / 5.0f);
        drops[i].length = 2.0f + (float)(rand() % 5);
    }

    void update() {
        // ÍáÞÉ ÊßÑÇÑ ÂãäÉ ÊÚÊãÏ Úáì ÍÌã ÇáÜ vector ÇáÍÞíÞí
        for (size_t i = 0; i < drops.size(); i++) {
            drops[i].y -= drops[i].speed;

            if (drops[i].y < 0) {
                resetDrop((int)i);
            }
        }
    }

    void draw() const {
        glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // íÍÇÝÙ Úáì ÅÚÏÇÏÇÊ ÇáÅÖÇÁÉ ÇáÃÕáíÉ
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);

        glBegin(GL_LINES);
        for (const auto& d : drops) {
            glColor4f(0.7f, 0.7f, 1.0f, 0.5f);
            glVertex3f(d.x, d.y, d.z);
            glVertex3f(d.x, d.y + d.length, d.z);
        }
        glEnd();

        glPopAttrib(); // íÚíÏ ÇáÅÖÇÁÉ ßãÇ ßÇäÊ ÈÏÞÉ
    }
};

#endif