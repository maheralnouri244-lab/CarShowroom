#include "Roadway.h"

void Roadway::setTextures(unsigned int road, unsigned int pavement, unsigned int parking, unsigned int grass) {
    roadTex = road; 
    pavementTex = pavement; 
    parkingTex = parking; 
    grassTex = grass;
}

void Roadway::drawInfiniteGround() {
    glEnable(GL_STENCIL_TEST);    
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); 
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
    glDepthMask(GL_FALSE);

    float lakeX = -280.0f; 
    float lakeZ = 600.0f; 
    float lakeW = 200.0f; 
    float lakeL = 300.0f;

    glBegin(GL_QUADS); 
    glVertex3f(lakeX - lakeW / 2, -0.5f, lakeZ - lakeL / 2); glVertex3f(lakeX + lakeW / 2, -0.5f, lakeZ - lakeL / 2);
    glVertex3f(lakeX + lakeW / 2, -0.5f, lakeZ + lakeL / 2); glVertex3f(lakeX - lakeW / 2, -0.5f, lakeZ + lakeL / 2); glEnd();
    
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); 
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); 
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 
    GLfloat mat_shininess[] = { 0.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); 
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, grassTex); 
    setupTex();

    glColor3f(1.0f, 1.0f, 1.0f); 
    float size = 6000.0f; 
    float y = -0.5f; 
    float scaleFactor = 0.1f;

    glBegin(GL_QUADS); 
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0f, 0.0f); 
    glVertex3f(-size, y, -size); 
    glTexCoord2f(size * scaleFactor, 0.0f); 
    glVertex3f(size, y, -size);
    glTexCoord2f(size * scaleFactor, size * scaleFactor); 
    glVertex3f(size, y, size); 
    glTexCoord2f(0.0f, size * scaleFactor); 
    glVertex3f(-size, y, size);
    glEnd(); 
    glDisable(GL_TEXTURE_2D);
    GLfloat default_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    GLfloat default_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular); 
    glMaterialfv(GL_FRONT, GL_SHININESS, default_shininess);
    glDisable(GL_STENCIL_TEST);
}

void Roadway::drawRoadMarkings(float x, float y, float z, float width, float length, bool isHorizontal) {
    glDisable(GL_TEXTURE_2D); 
    float yL = y + 0.03f; 
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS); 
    if (!isHorizontal) { 
        glVertex3f(x - width / 2 + 1, yL, z); 
        glVertex3f(x - width / 2 + 2, yL, z); 
        glVertex3f(x - width / 2 + 2, yL, z + length); 
        glVertex3f(x - width / 2 + 1, yL, z + length); 
        glVertex3f(x + width / 2 - 2, yL, z); 
        glVertex3f(x + width / 2 - 1, yL, z); 
        glVertex3f(x + width / 2 - 1, yL, z + length);
        glVertex3f(x + width / 2 - 2, yL, z + length);
    }
    else { 
        glVertex3f(x, yL, z - width / 2 + 1);
        glVertex3f(x + length, yL, z - width / 2 + 1);
        glVertex3f(x + length, yL, z - width / 2 + 2);
        glVertex3f(x, yL, z - width / 2 + 2); 
        glVertex3f(x, yL, z + width / 2 - 2); 
        glVertex3f(x + length, yL, z + width / 2 - 2);
        glVertex3f(x + length, yL, z + width / 2 - 1); 
        glVertex3f(x, yL, z + width / 2 - 1); 
    }
    glEnd();

    glColor3f(1, 0.8, 0); 
    float d = 8, g = 12; 
    glBegin(GL_QUADS); 
    if (!isHorizontal) {
        for (float c = z;c < z + length;c += d + g) {
            glVertex3f(x - 0.3, yL, c);
            glVertex3f(x + 0.3, yL, c); 
            glVertex3f(x + 0.3, yL, c + d); 
            glVertex3f(x - 0.3, yL, c + d); 
        } 
    }
    else {
        for (float c = x;c < x + length;c += d + g) {
            glVertex3f(c, yL, z - 0.3);
            glVertex3f(c + d, yL, z - 0.3); 
            glVertex3f(c + d, yL, z + 0.3);
            glVertex3f(c, yL, z + 0.3);
        }
    } 
    glEnd();
}

void Roadway::drawRoadSegment(float x, float y, float z, float width, float length, bool isHorizontal) {
    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, roadTex); 
    setupTex();

    glColor3f(1, 1, 1); 
    float t = 1.0f / 60.0f; 

    glBegin(GL_QUADS); 
    glNormal3f(0, 1, 0);

    if (!isHorizontal) {
        float u = width * t, v = length * t;
        glTexCoord2f(0, 0); 
        glVertex3f(x - width / 2, y, z); 
        glTexCoord2f(u, 0); 
        glVertex3f(x + width / 2, y, z); 
        glTexCoord2f(u, v); 
        glVertex3f(x + width / 2, y, z + length);
        glTexCoord2f(0, v); 
        glVertex3f(x - width / 2, y, z + length); 
    }
    else { 
        float u = length * t, v = width * t; 
        glTexCoord2f(0, 0); 
        glVertex3f(x, y, z - width / 2); 
        glTexCoord2f(u, 0); 
        glVertex3f(x + length, y, z - width / 2); 
        glTexCoord2f(u, v); 
        glVertex3f(x + length, y, z + width / 2); 
        glTexCoord2f(0, v); 
        glVertex3f(x, y, z + width / 2); 
    }
    glEnd(); 
    glDisable(GL_TEXTURE_2D); 
    drawRoadMarkings(x, y, z, width, length, isHorizontal);
}

void Roadway::drawSidewalk(float x, float y, float z, float width, float length, float height) {
    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, pavementTex); 
    setupTex();

    glColor3f(0.9f, 0.9f, 0.9f); 
    float t = 1.0f / 20.0f; 
    float u = width * t, v = length * t;

    glBegin(GL_QUADS); 
    glNormal3f(0, 1, 0); 
    glTexCoord2f(0, 0); 
    glVertex3f(x - width / 2, y + height, z - length / 2); 
    glTexCoord2f(u, 0); 
    glVertex3f(x + width / 2, y + height, z - length / 2); 
    glTexCoord2f(u, v); 
    glVertex3f(x + width / 2, y + height, z + length / 2); 
    glTexCoord2f(0, v); 
    glVertex3f(x - width / 2, y + height, z + length / 2); 
    glEnd(); 
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.5f, 0.5f, 0.5f); 
    glBegin(GL_QUAD_STRIP);

    float yT = y + height;
    glVertex3f(x - width / 2, y, z - length / 2);
    glVertex3f(x - width / 2, yT, z - length / 2); 
    glVertex3f(x + width / 2, y, z - length / 2); 
    glVertex3f(x + width / 2, yT, z - length / 2);
    glVertex3f(x + width / 2, y, z + length / 2); 
    glVertex3f(x + width / 2, yT, z + length / 2); 
    glVertex3f(x - width / 2, y, z + length / 2); 
    glVertex3f(x - width / 2, yT, z + length / 2); 
    glVertex3f(x - width / 2, y, z - length / 2);
    glVertex3f(x - width / 2, yT, z - length / 2); 

    glEnd();
}

void Roadway::drawCurvedSidewalk(float x, float y, float z, float radius, float startAngle, float endAngle, float width) {
    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, pavementTex);

    setupTex(); 
    glColor3f(0.9f, 0.9f, 0.9f);

    glBegin(GL_QUAD_STRIP); 
    for (int i = 0;i <= 20;i++) {
        float a = startAngle + (endAngle - startAngle) * ((float)i / 20);
        glNormal3f(0, 1, 0);
        glTexCoord2f((float)i / 20, 0);
        glVertex3f(x + radius * cos(a), y, z + radius * sin(a));
        glTexCoord2f((float)i / 20, 1); 
        glVertex3f(x + (radius + width) * cos(a), y, z + (radius + width) * sin(a));
    } 
    
    glEnd(); 
    glDisable(GL_TEXTURE_2D);
}

void Roadway::drawCrosswalk(float x, float y, float z, float width, float length, bool isHorizontal) {
    glDisable(GL_TEXTURE_2D); 
    glColor3f(1, 1, 1); 
    float s = 3, g = 3; 
    glBegin(GL_QUADS);

    if (!isHorizontal) 
        for (float c = z;c < z + length;c += s + g) { 
            glVertex3f(x - width / 2, y + 0.04, c); 
            glVertex3f(x + width / 2, y + 0.04, c); 
            glVertex3f(x + width / 2, y + 0.04, c + s); 
            glVertex3f(x - width / 2, y + 0.04, c + s); 
        }
    else for (float c = x;c < x + length;c += s + g) { 
        glVertex3f(c, y + 0.04, z - width / 2); 
        glVertex3f(c + s, y + 0.04, z - width / 2);
        glVertex3f(c + s, y + 0.04, z + width / 2);
        glVertex3f(c, y + 0.04, z + width / 2);
    } 
    glEnd();
}

void Roadway::drawParkingLines(float x, float z, float width, float length, int rows, int cols) {
    glColor3f(1, 1, 1); 
    glLineWidth(2); 
    glBegin(GL_LINES); 
    float s = width / cols;
    for (int i = 0; i <= cols; i++) {
        float c = x - width / 2 + i * s; 
        glVertex3f(c, 0.18f, z - length / 2);
        glVertex3f(c, 0.18f, z + length / 2); 
    } 
    glEnd();
}
