#include "Showroomside.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <cmath>

Showroomside::Showroomside(float MAXX, float MINX, float MAXZ, float MINZ,float h, float wx, float wy,float wz,int n,int n2){
    
    num = n;
    num2 = n2;
    MNX = MINX;
    MXX = MAXX;
    MNZ = MINZ;
    MXZ = MAXZ;
    height = h;
    widthx = wx;
    widthy = wy;
    widthz = wz;
    r = 0.05f; g = 0.05f; b = 0.1f;
}

void Showroomside::setColor(float red, float green, float blue) {
    r = red; g = green; b = blue;
}

void Showroomside::draw() const {
    float x, y, z;


    x = (MNX + MXX) / 2; y = 0; z = (MNZ + MXZ) / 2;
    for (int i = 0; i < num2; i++) {
        y += height / (num2+1);
        glPushMatrix();
        glTranslatef(x, y, z);
        glColor3f(r, g, b);
        glScalef((MXX==MNX?widthx:MXX-MNX), widthy, (MXZ == MNZ ? widthz : MXZ - MNZ));
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    y = height / 2;
    x = MNX;
    z = MNZ;
    for (int i = 0; i < num; i++) {

        x += (MXX-MNX+1) / (num+1);
        z += (MXZ - MNZ + 1) / (num + 1);
        if (MXX < x)
            x = MXX;
        if (MXZ < z)
            z = MXZ;
      
        glPushMatrix();
        glTranslatef(x, y, z);
        glColor3f(r, g, b);
        glScalef(widthx, height, widthz);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
}