#include "Vegetation.h"

Vegetation::Vegetation() {
    treeModel.load("MapleTree.obj");
}

void Vegetation::setTextures(unsigned int bark, unsigned int leaf) {
    treeModel.assignTexture("None.001", bark);
    treeModel.assignTexture("None", bark);
    treeModel.assignTexture("None_maple_leaf.png.001", leaf);
    treeModel.assignColor("None_maple_leaf.png.001", 0.4f, 0.85f, 0.4f);
    treeModel.assignColor("None.001", 0.6f, 0.3f, 0.1f);
}

void Vegetation::drawTree(float x, float y, float z) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glScalef(1.5f, 1.5f, 1.5f);

    int seed = (int)(x * z); 
    glRotatef(seed % 360, 0, 1, 0);

    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST); 
    glAlphaFunc(GL_GREATER, 0.5f);

    treeModel.draw();

    glDisable(GL_ALPHA_TEST); 
    glDisable(GL_BLEND);
    glPopMatrix();
}

void Vegetation::drawFlowerBed(float x, float y, float z) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.6f, 0.6f, 0.6f); 

    glPushMatrix(); 
    glScalef(10, 2, 10); 
    glutSolidCube(1); 
    glPopMatrix();

    glColor3f(0.3f, 0.2f, 0.1f); 
    glPushMatrix(); 
    glTranslatef(0, 1.1f, 0); 
    glScalef(9, 0.5, 9); 
    glutSolidCube(1); 
    glPopMatrix();

    for (int i = 0; i < 5; i++) {
        glPushMatrix(); 
        float fx = (rand() % 8) - 4; 
        float fz = (rand() % 8) - 4;

        glTranslatef(fx, 2.0f, fz); 
        if (i % 2 == 0) 
            glColor3f(1, 0, 0); 
        else 
            glColor3f(1, 1, 0);
        glutSolidSphere(0.5, 5, 5); 
        glPopMatrix();
    } 
    glPopMatrix();
}

void Vegetation::drawRock(float x, float y, float z, float size) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.5f, 0.5f, 0.55f); 
    glScalef(size, size * 0.7, size);
    glRotatef(rand() % 360, 0, 1, 0);
    glRotatef(rand() % 90, 1, 0, 0); 
    glutSolidDodecahedron(); 
    glPopMatrix();
}

void Vegetation::drawReed(float x, float y, float z) {
    glPushMatrix(); 
    glTranslatef(x, y, z); 
    glDisable(GL_TEXTURE_2D); 
    glColor3f(0.2f, 0.4f, 0.1f);
    for (int i = 0; i < 5; i++) { 
        glPushMatrix(); 
        glTranslatef((rand() % 10) / 10.0f, 0, (rand() % 10) / 10.0f); 
        glScalef(0.05f, 1.5f + (rand() % 10) / 10.0f, 0.05f); 
        glutSolidCube(1); 
        glPopMatrix(); 
    }
    glColor3f(0.4f, 0.2f, 0.1f); 
    glTranslatef(0, 1.5f, 0); 
    glScalef(0.15f, 0.6f, 0.15f); 
    glutSolidSphere(1, 5, 5); 
    glPopMatrix();
}