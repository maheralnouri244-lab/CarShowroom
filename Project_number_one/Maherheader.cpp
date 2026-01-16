#include <GL/glut.h>
#include <vector>
#include <cmath>
#include "Pillar.h"
#include "Hpillar.h"
#include "Showroomside.h"
#include "ShowroomGate.h"
#include "GlassWindow.h"
#include "PlusRing3D.h"
#include "Maherheader.h"
#include <utility>
#include "StageLight.h"
#include "NeonTube.h"
#include "SteveModel.h"
#include <iostream>

using namespace std;

Maherheader::Maherheader() {}

void Maherheader::draw() const {

	PlusRing3D symbol;
	vector<Pillar> pillars;
	vector<Hpillar> hpillars;
	vector<Showroomside> showroomsides;
	vector<GlassWindow> glasswindows;
    vector< NeonTube> neonyubes;
    StageLight light;
	float maxX = 150, maxz = 200, diff = 45;
	float minX = -maxX, minz = -maxz;
	pair<float, float> p[4] = {{maxX,maxz},{minX,maxz},{maxX,minz},{minX,minz} };
	int h = 60;


    glColor3f(0.35f, 0.65f, 0.20f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1000.0f, -0.2f, 1000.0f);
    glVertex3f(1000.0f, -0.2f, -1000.0f);
    glVertex3f(-1000.0f, -0.2f, -1000.0f);
    glVertex3f(-1000.0f, -0.2f, 1000.0f);
    glEnd();



    for (int i = 0; i < 1; i++) {
        //cout << p[i].first << " " << p[i].second << "\n";
        pillars.push_back(Pillar(p[i].first, 0, p[i].second, h));
    }

    hpillars.push_back(Hpillar(maxX, 0, 0, 0.4, 0.8, maxz - minz));
    hpillars.push_back(Hpillar(maxX, h - 0.4, 0, 0.4, 0.8, maxz - minz));
    hpillars.push_back(Hpillar(minX, 0, 0, 0.4, 0.8, maxz - minz));
    hpillars.push_back(Hpillar(minX, h - 0.4, 0, 0.4, 0.8, maxz - minz));
    hpillars.push_back(Hpillar(0, 0, minz, 0.4, maxX - minX, 0.8));
    hpillars.push_back(Hpillar(0, h - 0.4, minz, 0.4, maxX - minX, 0.8));
    hpillars.push_back(Hpillar(0, h - 0.4, maxz, 0.4, maxX - minX, 0.8));
    hpillars.push_back(Hpillar(minX + (maxX - minX - diff) / 4, 0, maxz, 0.4, (maxX - minX - diff) / 2, 0.8));
    hpillars.push_back(Hpillar(maxX - (maxX - minX - diff) / 4, 0, maxz, 0.4, (maxX - minX - diff) / 2, 0.8));

    // ground line
    hpillars.push_back(Hpillar(maxX - (maxX - minX - diff) / 2, 0, 0, 0.4, 0.8, maxz - minz - 2 * diff));
    hpillars.push_back(Hpillar(minX + (maxX - minX - diff) / 2, 0, 0, 0.4, 0.8, maxz - minz - 2 * diff));

    // roof line
    hpillars.push_back(Hpillar(maxX - (maxX - minX - diff) / 2, h, 0, 0.4, 0.8, maxz - minz - 2 * diff));
    hpillars.push_back(Hpillar(minX + (maxX - minX - diff) / 2, h, 0, 0.4, 0.8, maxz - minz - 2 * diff));

    showroomsides.push_back(Showroomside(maxX, minX, minz, minz, h, 0.2, 0.2, 0.2, 40, 5));
    showroomsides.push_back(Showroomside(minX, minX, maxz, minz, h, 0.2, 0.2, 0.2, 40, 5));
    showroomsides.push_back(Showroomside(maxX, maxX, maxz, minz, h, 0.2, 0.2, 0.2, 40, 5));
    showroomsides.push_back(Showroomside(minX + (maxX - minX - diff) / 2, minX, maxz, maxz, h, 0.2, 0.2, 0.2, 18, 5));
    showroomsides.push_back(Showroomside(maxX, maxX - (maxX - minX - diff) / 2, maxz, maxz, h, 0.2, 0.2, 0.2, 18, 5));

    //ground line
    showroomsides.push_back(Showroomside(minX + (maxX - minX - diff) / 2, minX + (maxX - minX - diff) / 2, (maxz - minz - 2 * diff) / 2, -(maxz - minz - 2 * diff) / 2, h, 0.2, 0.2, 0.2, 18, 5));
    showroomsides.push_back(Showroomside(maxX - (maxX - minX - diff) / 2, maxX - (maxX - minX - diff) / 2, (maxz - minz - 2 * diff) / 2, -(maxz - minz - 2 * diff) / 2, h, 0.2, 0.2, 0.2, 18, 5));

    glasswindows.push_back(GlassWindow(maxX, 0, minz, maxX, h, minz, minX, h, minz, minX, 0, minz));
    glasswindows.push_back(GlassWindow(maxX, 0, minz, maxX, h, minz, maxX, h, maxz, maxX, 0, maxz));
    glasswindows.push_back(GlassWindow(minX, 0, minz, minX, h, minz, minX, h, maxz, minX, 0, maxz));
    glasswindows.push_back(GlassWindow(minX, 0, maxz, minX, h, maxz, minX + (maxX - minX - diff) / 2 + 3, h, maxz, minX + (maxX - minX - diff) / 2 + 3, 0, maxz));
    glasswindows.push_back(GlassWindow(maxX, 0, maxz, maxX, h, maxz, maxX - (maxX - minX - diff) / 2 - 3, h, maxz, maxX - (maxX - minX - diff) / 2 - 3, 0, maxz));

    //ground line
    glasswindows.push_back(GlassWindow(minX + (maxX - minX - diff) / 2, 0, (maxz - minz - 2 * diff) / 2, minX + (maxX - minX - diff) / 2, h, (maxz - minz - 2 * diff) / 2, minX + (maxX - minX - diff) / 2, h, -(maxz - minz - 2 * diff) / 2, minX + (maxX - minX - diff) / 2, 0, -(maxz - minz - 2 * diff) / 2));
    glasswindows.push_back(GlassWindow(maxX - (maxX - minX - diff) / 2, 0, (maxz - minz - 2 * diff) / 2, maxX - (maxX - minX - diff) / 2, h, (maxz - minz - 2 * diff) / 2, maxX - (maxX - minX - diff) / 2, h, -(maxz - minz - 2 * diff) / 2, maxX - (maxX - minX - diff) / 2, 0, -(maxz - minz - 2 * diff) / 2));


    neonyubes.push_back(NeonTube(minX + (maxX - minX - diff) / 2 + 4, 0, 0, maxz - minz - diff, 0.1, 'z'));
    neonyubes.push_back(NeonTube(maxX - (maxX - minX - diff) / 2 - 4, 0, 0, maxz - minz - diff, 0.1, 'z'));
    neonyubes.push_back(NeonTube(0, 0, (maxz - minz - diff) / 2, (maxX - minX - diff/2), 0.1, 'x'));
    neonyubes.push_back(NeonTube(0, 0, -(maxz - minz - diff) / 2, (maxX - minX - diff/2), 0.1, 'x'));
    neonyubes.push_back(NeonTube(minX + (maxX - minX - diff) / 4, 0, (maxz + minz) / 2 +  2*diff, (maxz - minz) / 2 - 2*diff, 0.1, 'z'));
    neonyubes.push_back(NeonTube(minX + (maxX - minX - diff) / 4, 0, - (maxz + minz) / 2 - 2*diff, (maxz - minz) / 2 - 2*diff, 0.1, 'z'));
    neonyubes.push_back(NeonTube(-minX - (maxX - minX - diff) / 4, 0, (maxz + minz) / 2 + 2 * diff, (maxz - minz) / 2 - 2 * diff, 0.1, 'z'));
    neonyubes.push_back(NeonTube(-minX - (maxX - minX - diff) / 4, 0, -(maxz + minz) / 2 - 2 * diff, (maxz - minz) / 2 - 2 * diff, 0.1, 'z'));

    
    for (const auto& p : pillars) {
        p.draw();
    }

    for (const auto& p : hpillars) {
        p.draw();
    }

    for (const auto& p : showroomsides) {
        p.draw();
    }

    for (const auto& p : neonyubes) {
        p.draw();
    }

    ShowroomGate showroomgate;
    showroomgate.draw();
    
    glPushMatrix();
    glTranslatef(0, 0, (maxz - minz - 2 * diff) / 2);
    showroomgate.drawGateArch();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -(maxz - minz - 2 * diff) / 2);
    showroomgate.drawGateArch();
    glPopMatrix();

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(maxX, h, maxz);
    glVertex3f(maxX, h, minz);
    glVertex3f(minX, h, minz);
    glVertex3f(minX, h, maxz);
    glEnd();

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(maxX, 0, maxz);
    glVertex3f(maxX, 0, minz);
    glVertex3f(minX, 0, minz);
    glVertex3f(minX, 0, maxz);
    glEnd();

    int cnt = 5;
    for (int i = 0; i <= cnt; i++) {
        NeonTube C1 = NeonTube(-minX - (maxX - minX - diff) / 4, (30.0/cnt)*i, 0, (maxz - minz) / 2 - 2 * diff, 0.1, 'z');
        C1.drawC(0.2, 40);
        if (i == 0)
            C1.drawColumns(40, 12, 30, 0.2, 3, 1);
    }

    for (int i = 0; i <= cnt; i++) {
        NeonTube C2 = NeonTube(minX + (maxX - minX - diff) / 4, (30.0 / cnt) * i, 0, (maxz - minz) / 2 - 2 * diff, 0.1, 'z');
        C2.drawC(0.2, 40);
        if (i == 0)
            C2.drawColumns(40, 12, 30, 0.2, 3, 1);
    }

    // steves draw
    float groupBaseX = minX + (maxX - minX - diff) / 4.0f;
    float steveData[41][15] = {
    { -40.0f, 230.0f, 180.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.00f, 0.65f, 0.65f, 0.25f, 0.25f, 0.60f },
    { 40.0f, 250.0f, 170.0f, 0.80f, 0.55f, 0.40f, 0.20f, 0.20f, 0.20f, 0.80f, 0.20f, 0.20f, 0.10f, 0.10f, 0.40f },
    { 60.0f, 280.0f, 190.0f, 0.60f, 0.40f, 0.30f, 0.10f, 0.10f, 0.10f, 0.20f, 0.70f, 0.20f, 0.30f, 0.30f, 0.30f },
    { -80.0f, 300.0f, 160.0f, 0.75f, 0.50f, 0.38f, 0.30f, 0.20f, 0.10f, 0.90f, 0.90f, 0.20f, 0.20f, 0.20f, 0.50f },
    { 100.0f, 240.0f, 200.0f, 0.50f, 0.35f, 0.25f, 0.10f, 0.05f, 0.05f, 0.50f, 0.50f, 0.50f, 0.15f, 0.15f, 0.20f },
    { -120.0f, 260.0f, 180.0f, 0.70f, 0.47f, 0.35f, 0.40f, 0.30f, 0.20f, 0.20f, 0.20f, 0.80f, 0.10f, 0.10f, 0.10f },
    { 140.0f, 350.0f, 175.0f, 0.85f, 0.60f, 0.45f, 0.60f, 0.50f, 0.30f, 0.70f, 0.40f, 0.20f, 0.30f, 0.30f, 0.50f },
    { 20.0f, -230.0f, 0.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.40f, 0.10f, 0.60f, 0.20f, 0.20f, 0.20f },
    { -50.0f, -260.0f, 10.0f, 0.65f, 0.45f, 0.30f, 0.15f, 0.15f, 0.15f, 0.20f, 0.60f, 0.80f, 0.25f, 0.25f, 0.40f },
    { 90.0f, -290.0f, 350.0f, 0.55f, 0.38f, 0.28f, 0.05f, 0.05f, 0.05f, 0.80f, 0.80f, 0.80f, 0.20f, 0.20f, 0.30f },
    { -10.0f, -320.0f, 5.0f, 0.72f, 0.48f, 0.36f, 0.35f, 0.25f, 0.15f, 0.10f, 0.50f, 0.10f, 0.40f, 0.40f, 0.30f },
    { -130.0f, -240.0f, 340.0f, 0.80f, 0.55f, 0.40f, 0.50f, 0.40f, 0.20f, 0.90f, 0.50f, 0.10f, 0.20f, 0.20f, 0.50f },
    { 110.0f, -270.0f, 15.0f, 0.60f, 0.40f, 0.30f, 0.20f, 0.10f, 0.05f, 0.30f, 0.30f, 0.60f, 0.15f, 0.15f, 0.25f },
    { -90.0f, -380.0f, 0.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.70f, 0.20f, 0.40f, 0.25f, 0.25f, 0.60f },
    { 180.0f, 0.0f, 270.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.00f, 0.65f, 0.65f, 0.25f, 0.25f, 0.60f },
    { 200.0f, 80.0f, 260.0f, 0.50f, 0.35f, 0.25f, 0.10f, 0.10f, 0.10f, 0.50f, 0.10f, 0.10f, 0.10f, 0.10f, 0.10f },
    { 220.0f, -60.0f, 280.0f, 0.80f, 0.55f, 0.40f, 0.40f, 0.30f, 0.10f, 0.20f, 0.60f, 0.20f, 0.20f, 0.30f, 0.20f },
    { 250.0f, 120.0f, 275.0f, 0.70f, 0.47f, 0.35f, 0.30f, 0.20f, 0.15f, 0.10f, 0.40f, 0.70f, 0.30f, 0.30f, 0.40f },
    { 190.0f, -150.0f, 265.0f, 0.65f, 0.45f, 0.30f, 0.20f, 0.10f, 0.05f, 0.60f, 0.60f, 0.20f, 0.20f, 0.20f, 0.30f },
    { 280.0f, 40.0f, 255.0f, 0.75f, 0.50f, 0.38f, 0.50f, 0.20f, 0.10f, 0.80f, 0.40f, 0.40f, 0.30f, 0.10f, 0.10f },
    { 320.0f, -90.0f, 290.0f, 0.55f, 0.38f, 0.28f, 0.05f, 0.05f, 0.05f, 0.40f, 0.40f, 0.40f, 0.10f, 0.10f, 0.20f },
    { 210.0f, 180.0f, 270.0f, 0.82f, 0.60f, 0.50f, 0.60f, 0.50f, 0.20f, 0.20f, 0.20f, 0.50f, 0.15f, 0.15f, 0.25f },
    { -180.0f, 20.0f, 90.0f, 0.60f, 0.40f, 0.30f, 0.15f, 0.15f, 0.15f, 0.30f, 0.70f, 0.70f, 0.10f, 0.10f, 0.30f },
    { -210.0f, -50.0f, 85.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.70f, 0.30f, 0.30f, 0.25f, 0.25f, 0.40f },
    { -230.0f, 100.0f, 95.0f, 0.80f, 0.55f, 0.40f, 0.45f, 0.35f, 0.25f, 0.40f, 0.20f, 0.60f, 0.20f, 0.20f, 0.20f },
    { -260.0f, -120.0f, 80.0f, 0.72f, 0.48f, 0.36f, 0.30f, 0.20f, 0.10f, 0.20f, 0.60f, 0.20f, 0.20f, 0.40f, 0.20f },
    { -195.0f, 160.0f, 100.0f, 0.50f, 0.35f, 0.25f, 0.10f, 0.10f, 0.10f, 0.90f, 0.50f, 0.10f, 0.30f, 0.30f, 0.40f },
    { -300.0f, -20.0f, 90.0f, 0.75f, 0.50f, 0.38f, 0.55f, 0.45f, 0.35f, 0.30f, 0.30f, 0.80f, 0.10f, 0.10f, 0.30f },
    { -220.0f, 190.0f, 90.0f, 0.65f, 0.45f, 0.30f, 0.20f, 0.10f, 0.10f, 0.50f, 0.50f, 0.20f, 0.20f, 0.30f, 0.20f },
    { -280.0f, -180.0f, 110.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.60f, 0.20f, 0.20f, 0.15f, 0.15f, 0.25f },
    { groupBaseX + 0.0f,   0.0f,   0.0f,   0.70f, 0.47f, 0.35f,  0.25f, 0.15f, 0.10f,  0.10f, 0.50f, 0.80f,  0.20f, 0.20f, 0.50f },
    { groupBaseX + 12.0f,  8.0f,   140.0f, 0.80f, 0.55f, 0.40f,  0.10f, 0.10f, 0.10f,  0.90f, 0.20f, 0.20f,  0.15f, 0.15f, 0.25f },
    { groupBaseX - 12.0f,  5.0f,   30.0f,  0.60f, 0.40f, 0.30f,  0.35f, 0.25f, 0.15f,  0.20f, 0.70f, 0.20f,  0.30f, 0.30f, 0.30f },
    { groupBaseX + 2.0f,  -10.0f,  200.0f, 0.75f, 0.50f, 0.38f,  0.20f, 0.20f, 0.20f,  0.50f, 0.50f, 0.50f,  0.10f, 0.10f, 0.10f },
    { -groupBaseX + 0.0f,   5.0f,   180.0f, 0.70f, 0.47f, 0.35f, 0.25f, 0.15f, 0.10f, 0.80f, 0.10f, 0.10f, 0.10f, 0.10f, 0.40f },
    { -groupBaseX + 10.0f, -5.0f,   210.0f, 0.80f, 0.55f, 0.40f, 0.10f, 0.10f, 0.10f, 0.20f, 0.60f, 0.20f, 0.20f, 0.30f, 0.20f },
    { -groupBaseX - 8.0f,  15.0f,   160.0f, 0.60f, 0.40f, 0.30f, 0.35f, 0.25f, 0.15f, 0.50f, 0.50f, 0.10f, 0.30f, 0.10f, 0.10f },
    //{ -groupBaseX + 4.0f,  25.0f,   190.0f, 0.75f, 0.50f, 0.38f, 0.20f, 0.20f, 0.20f, 0.10f, 0.10f, 0.70f, 0.25f, 0.25f, 0.25f },
    { -groupBaseX - 12.0f, -15.0f,  300.0f, 0.50f, 0.35f, 0.25f, 0.10f, 0.05f, 0.05f, 0.70f, 0.30f, 0.80f, 0.15f, 0.15f, 0.20f },
    { -groupBaseX + 15.0f, 10.0f,   45.0f,  0.85f, 0.60f, 0.45f, 0.60f, 0.50f, 0.30f, 0.30f, 0.70f, 0.30f, 0.10f, 0.40f, 0.10f },
    };

    SteveModel steve(0.3f);

    for (int i = 0; i < 39; i++)
    {
        SteveModel::setSkinColor(steveData[i][3], steveData[i][4], steveData[i][5]);
        SteveModel::setHairColor(steveData[i][6], steveData[i][7], steveData[i][8]);
        SteveModel::setShirtColor(steveData[i][9], steveData[i][10], steveData[i][11]);
        SteveModel::setPantsColor(steveData[i][12], steveData[i][13], steveData[i][14]);

        glPushMatrix();
        glTranslatef(steveData[i][0], 0.0f, steveData[i][1]);
        glRotatef(steveData[i][2], 0.0f, 1.0f, 0.0f);
        steve.draw();
        glPopMatrix();
    }


    float roadStartCoord = maxz;      
    float roadEndCoord = 1000.0f;     
    float roadCenterLine = 0.0f;      
    float roadWidth = diff;           
    float halfW = roadWidth / 2.0f;
    float roadY = 0.01f;              

    glColor3f(0.15f, 0.15f, 0.15f);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-halfW, roadY, roadStartCoord);
    glVertex3f(halfW, roadY, roadStartCoord);
    glVertex3f(halfW, roadY, roadEndCoord);
    glVertex3f(-halfW, roadY, roadEndCoord);
    glEnd();

    float sY = roadY + 0.01f;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex3f(halfW - 1.0f, sY, roadStartCoord);
    glVertex3f(halfW - 0.5f, sY, roadStartCoord);
    glVertex3f(halfW - 0.5f, sY, roadEndCoord);
    glVertex3f(halfW - 1.0f, sY, roadEndCoord);

    glVertex3f(-halfW + 0.5f, sY, roadStartCoord);
    glVertex3f(-halfW + 1.0f, sY, roadStartCoord);
    glVertex3f(-halfW + 1.0f, sY, roadEndCoord);
    glVertex3f(-halfW + 0.5f, sY, roadEndCoord);
    glEnd();

    glColor3f(1.0f, 0.8f, 0.0f);
    float dLen = 10.0f;
    float dGap = 10.0f;
    float dWidth = 1.0f;

    glBegin(GL_QUADS);
    for (float z = roadStartCoord; z < roadEndCoord; z += (dLen + dGap)) {
        glVertex3f(-dWidth / 2.0f, sY, z);
        glVertex3f(dWidth / 2.0f, sY, z);
        glVertex3f(dWidth / 2.0f, sY, z + dLen);
        glVertex3f(-dWidth / 2.0f, sY, z + dLen);
    }
    glEnd();

    float swWidth = 8.0f; 
    float swHeight = 2.0f;
    float swStart = maxz; 
    float swEnd = 1000.0f;
    float roadEdge = diff / 2.0f;

    glColor3f(0.6f, 0.6f, 0.6f);

    glBegin(GL_QUADS);

    glNormal3f(0, 1, 0);
    glVertex3f(roadEdge, swHeight, swStart);
    glVertex3f(roadEdge + swWidth, swHeight, swStart);
    glVertex3f(roadEdge + swWidth, swHeight, swEnd);
    glVertex3f(roadEdge, swHeight, swEnd);

    glNormal3f(-1, 0, 0);
    glVertex3f(roadEdge, 0, swStart);
    glVertex3f(roadEdge, swHeight, swStart);
    glVertex3f(roadEdge, swHeight, swEnd);
    glVertex3f(roadEdge, 0, swEnd);

    glNormal3f(0, 1, 0);
    glVertex3f(-roadEdge, swHeight, swStart);
    glVertex3f(-roadEdge - swWidth, swHeight, swStart);
    glVertex3f(-roadEdge - swWidth, swHeight, swEnd);
    glVertex3f(-roadEdge, swHeight, swEnd);

    glNormal3f(1, 0, 0);
    glVertex3f(-roadEdge, 0, swStart);
    glVertex3f(-roadEdge, swHeight, swStart);
    glVertex3f(-roadEdge, swHeight, swEnd);
    glVertex3f(-roadEdge, 0, swEnd);

    glEnd();


    for (const auto& p : glasswindows) {
        p.draw();
    }
}