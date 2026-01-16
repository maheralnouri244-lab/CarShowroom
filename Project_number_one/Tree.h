#ifndef TREE_H
#define TREE_H

#include <GL/glut.h>

class Tree {
public:
    Tree();
    void draw(float x, float y, float z) const;

private:
    void drawTrunk(float radius, float height) const;
    void drawLeaves(float radius, float height) const;
};

#endif