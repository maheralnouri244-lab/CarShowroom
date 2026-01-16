#include "Tree.h"

Tree::Tree() {}

void Tree::drawTrunk(float radius, float height) const {
    // ·Ê‰ »‰Ì ··Ã–⁄
    glColor3f(0.45f, 0.25f, 0.10f);

    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glRotatef(-90, 1, 0, 0); //  œÊÌ— «·√”ÿÊ«‰… · ’»Õ ﬁ«∆„…
    gluCylinder(quad, radius, radius, height, 16, 16);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void Tree::drawLeaves(float radius, float height) const {
    // ·Ê‰ √Œ÷— €«„ﬁ ··√Ê—«ﬁ
    glColor3f(0.1f, 0.4f, 0.1f);

    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    // «·„Œ—Êÿ: «·ﬁ«⁄œ… ·Â« ‰’› ﬁÿ— Ê«·ﬁ„…  ‰ ÂÌ ⁄‰œ «·’›—
    gluCylinder(quad, radius, 0.0f, height, 16, 16);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void Tree::draw(float x, float y, float z) const {
    glPushMatrix();
    glTranslatef(x, y, z);

    // 1. —”„ «·Ã–⁄ (‰’› ﬁÿ— 0.4 ÊÿÊ· 2.0)
    drawTrunk(0.4f, 2.0f);

    // 2. —”„ ÿ»ﬁ«  «·√Ê—«ﬁ ›Êﬁ »⁄÷Â«
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f); // »œ«Ì… «·ÿ»ﬁ… «·√Ê·Ï
    drawLeaves(1.5f, 2.0f);

    glTranslatef(0.0f, 1.2f, 0.0f); // »œ«Ì… «·ÿ»ﬁ… «·À«‰Ì…
    drawLeaves(1.2f, 1.8f);

    glTranslatef(0.0f, 1.0f, 0.0f); // »œ«Ì… «·ÿ»ﬁ… «·À«·À…
    drawLeaves(0.8f, 1.5f);
    glPopMatrix();

    glPopMatrix();
}