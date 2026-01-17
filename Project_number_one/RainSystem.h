#ifndef RAIN_SYSTEM_H
#define RAIN_SYSTEM_H

#include <GL/glut.h>
#include <vector>
#include <cstdlib> // ÖÑæÑí áÜ rand

struct RainDrop {
    float x, y, z;
    float speed;
    float length;
};

class RainSystem {
private:
    std::vector<RainDrop> drops;
    // ÍĞİäÇ numDrops ßãÊÛíÑ ãäİÕá áäÚÊãÏ Úáì drops.size() ãÈÇÔÑÉ

public:
    RainSystem(int count = 1000) {
        // ÇáÍá ÇáÓÍÑí: resize ÊŞæã ÈÍÌÒ ÇáÃãÇßä İÚáíÇğ İí ÇáĞÇßÑÉ
        drops.resize(count);

        for (int i = 0; i < (int)drops.size(); i++) {
            resetDrop(i);
            // ÊæÒíÚ ÚÔæÇÆí Ãæáí áßí áÇ íÓŞØ ÇáãØÑ ßÎØ æÇÍÏ
            drops[i].y = (float)(rand() % 500);
        }
    }

    void resetDrop(int i) {
        // ÇáÂä ÇáæÕæá áÜ drops[i] Âãä áÃääÇ ÇÓÊÚãáäÇ resize
        drops[i].x = (float)((rand() % 2000) - 1000); // æÓÚäÇ ÇáäØÇŞ áíÛØí ÎÑíØÊß
        drops[i].y = 400.0f + (rand() % 100);
        drops[i].z = (float)((rand() % 2000) - 1000);
        drops[i].speed = 2.0f + (float)(rand() % 10 / 5.0f);
        drops[i].length = 2.0f + (float)(rand() % 5);
    }

    void update() {
        // ÍáŞÉ ÊßÑÇÑ ÂãäÉ ÊÚÊãÏ Úáì ÍÌã ÇáÜ vector ÇáÍŞíŞí
        for (size_t i = 0; i < drops.size(); i++) {
            drops[i].y -= drops[i].speed;

            if (drops[i].y < 0) {
                resetDrop((int)i);
            }
        }
    }

    void draw() const {
        glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // íÍÇİÙ Úáì ÅÚÏÇÏÇÊ ÇáÅÖÇÁÉ ÇáÃÕáíÉ
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);

        glBegin(GL_LINES);
        for (const auto& d : drops) {
            glColor4f(0.7f, 0.7f, 1.0f, 0.5f);
            glVertex3f(d.x, d.y, d.z);
            glVertex3f(d.x, d.y + d.length, d.z);
        }
        glEnd();

        glPopAttrib(); // íÚíÏ ÇáÅÖÇÁÉ ßãÇ ßÇäÊ ÈÏŞÉ
    }
};

#endif