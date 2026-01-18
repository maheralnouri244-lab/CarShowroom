#include "NormalCar.h"

// مصفوفة الألوان المخصصة لسيارات سارة
const float SARA_PALETTE[4][3] = {
    { 0.85f, 0.1f, 0.1f }, // أحمر
    { 0.1f, 0.3f, 0.8f },  // أزرق
    { 0.9f, 0.8f, 0.1f },  // أصفر
    { 0.7f, 0.7f, 0.7f }   // فضي
};

NormalCar::NormalCar() {}

void NormalCar::drawBox(float w, float h, float l) {
    glPushMatrix();
    glScalef(w, h, l);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void NormalCar::drawProfessionalWheel() {
    float tireRadius = 0.32f, rimRadius = 0.22f;
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glColor3f(0.02f, 0.02f, 0.02f); // لون الإطار أسود
    glutSolidTorus(0.12, tireRadius, 15, 30);
    glColor3f(0.7f, 0.7f, 0.75f); // لون الجنط فضي
    glPushMatrix();
    glTranslatef(0, 0, 0.05f);
    glutSolidTorus(0.02, rimRadius, 10, 24);
    glPopMatrix();
    // رسم أسلاك الجنط (Spokes)
    for (int i = 0; i < 6; i++) {
        glPushMatrix();
        glRotatef(i * 60.0f, 0, 0, 1);
        glTranslatef(0, rimRadius / 2.0f, 0.02f);
        glScalef(0.05f, rimRadius, 0.04f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    glPopMatrix();
}

void NormalCar::drawSportRoof() {
    float w1 = 0.75f, w2 = 0.55f;
    float zFBot = 1.0f, zFTop = 0.4f, zBTop = -0.8f, zBBot = -1.3f, h = 0.5f;

    glBegin(GL_QUADS);
    // الزجاج الأمامي والخلفي
    glColor3f(0.2f, 0.3f, 0.4f);
    glVertex3f(-w1, 0, zFBot); glVertex3f(w1, 0, zFBot);
    glVertex3f(w2, h, zFTop);  glVertex3f(-w2, h, zFTop);
    glVertex3f(-w2, h, zBTop);  glVertex3f(w2, h, zBTop);
    glVertex3f(w1, 0, zBBot);   glVertex3f(-w1, 0, zBBot);

    // الأعمدة الجانبية (سوداء)
    glColor3f(0.02f, 0.02f, 0.02f);
    glVertex3f(w1, 0, zFBot);  glVertex3f(w1, 0, zBBot);
    glVertex3f(w2, h, zBTop);   glVertex3f(w2, h, zFTop);
    glVertex3f(-w1, 0, zFBot); glVertex3f(-w2, h, zFTop);
    glVertex3f(-w2, h, zBTop);  glVertex3f(-w1, 0, zBBot);
    glEnd();
}

void NormalCar::draw(float x, float y, float z, float scale, float rotateY, int colorIndex, float doorAngle) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotateY, 0, 1, 0);
    glScalef(scale, scale, scale);

    // 1. جسم السيارة الأساسي بلون القائمة
    glColor3fv(SARA_PALETTE[colorIndex % 4]);
    glPushMatrix();
    glTranslatef(0.0f, 0.65f, 0.0f);
    drawBox(1.7f, 0.5f, 4.0f);
    glPopMatrix();

    // 2. السقف الرياضي
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, -0.2f);
    drawSportRoof();
    glPopMatrix();

    // 3. الشبكة الأمامية (Grill) والمرايا
    glColor3f(0.02f, 0.02f, 0.02f);
    glPushMatrix(); glTranslatef(0.0f, 0.62f, 2.01f); drawBox(1.2f, 0.2f, 0.02f); glPopMatrix();

    glColor3fv(SARA_PALETTE[colorIndex % 4]);
    glPushMatrix(); glTranslatef(0.85f, 0.95f, 0.6f); drawBox(0.2f, 0.1f, 0.15f); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.85f, 0.95f, 0.6f); drawBox(0.2f, 0.1f, 0.15f); glPopMatrix();

    // 4. الجناح الخلفي (Spoiler)
    glPushMatrix(); glTranslatef(0.0f, 1.25f, -2.0f); glScalef(1.8f, 0.05f, 0.5f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.02f, 0.02f, 0.02f);
    glPushMatrix(); glTranslatef(0.6f, 1.05f, -1.9f); drawBox(0.08f, 0.4f, 0.1f); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.6f, 1.05f, -1.9f); drawBox(0.08f, 0.4f, 0.1f); glPopMatrix();

    // 5. الأضواء والعوادم
    glColor3f(1.0f, 1.0f, 0.8f); // Lights
    glPushMatrix(); glTranslatef(0.6f, 0.75f, 1.95f); glutSolidSphere(0.12f, 16, 16); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.6f, 0.75f, 1.95f); glutSolidSphere(0.12f, 16, 16); glPopMatrix();

    glColor3f(0.85f, 0.85f, 0.85f); // Chrome Exhausts
    glPushMatrix(); glTranslatef(0.4f, 0.5f, -2.01f); glutSolidSphere(0.07f, 10, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.4f, 0.5f, -2.01f); glutSolidSphere(0.07f, 10, 10); glPopMatrix();

    // 6. العجلات
    float wX = 0.88f, wY = 0.32f, wZF = 1.3f, wZR = -1.3f;
    glPushMatrix(); glTranslatef(wX, wY, wZF); drawProfessionalWheel(); glPopMatrix();
    glPushMatrix(); glTranslatef(-wX, wY, wZF); glRotatef(180, 0, 1, 0); drawProfessionalWheel(); glPopMatrix();
    glPushMatrix(); glTranslatef(wX, wY, wZR); drawProfessionalWheel(); glPopMatrix();
    glPushMatrix(); glTranslatef(-wX, wY, wZR); glRotatef(180, 0, 1, 0); drawProfessionalWheel(); glPopMatrix();

    glPopMatrix();
}