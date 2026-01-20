#include "CityMasterPlan.h"
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

CityMasterPlan::CityMasterPlan() : streetLightObj(22.0f, 0.7f) { // ارتفاع 22، نصف قطر 0.7
    roadTex = 0; pavementTex = 0; grassTex = 0; parkingTex = 0; waterTex = 0; wallTex = 0;
}

void CityMasterPlan::setTextures(unsigned int road, unsigned int pavement, unsigned int grass, unsigned int parkingAsphalt, unsigned int water, unsigned int wall) {
    roadTex = road; pavementTex = pavement; grassTex = grass; parkingTex = parkingAsphalt; waterTex = water; wallTex = wall;
}

void setupRepeatedTexture() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

// ==========================================
// العناصر التجميلية
// ==========================================

void CityMasterPlan::drawSimpleTree(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.4f, 0.26f, 0.13f); glPushMatrix(); glRotatef(-90, 1, 0, 0); glutSolidCone(2, 15, 10, 5); glPopMatrix();
    glColor3f(0.0f, 0.5f, 0.0f); glPushMatrix(); glTranslatef(0, 8, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(7, 12, 10, 5); glPopMatrix();
    glColor3f(0.0f, 0.6f, 0.0f); glPushMatrix(); glTranslatef(0, 12, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(6, 10, 10, 5); glPopMatrix();
    glColor3f(0.1f, 0.8f, 0.1f); glPushMatrix(); glTranslatef(0, 16, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(4, 8, 10, 5); glPopMatrix();
    glPopMatrix();
}

void CityMasterPlan::drawFlowerBed(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.6f, 0.6f, 0.6f); glPushMatrix(); glScalef(10, 2, 10); glutSolidCube(1); glPopMatrix();
    glColor3f(0.3f, 0.2f, 0.1f); glPushMatrix(); glTranslatef(0, 1.1f, 0); glScalef(9, 0.5, 9); glutSolidCube(1); glPopMatrix();
    for (int i = 0; i < 5; i++) {
        glPushMatrix(); float fx = (rand() % 8) - 4; float fz = (rand() % 8) - 4;
        glTranslatef(fx, 2.0f, fz); if (i % 2 == 0) glColor3f(1, 0, 0); else glColor3f(1, 1, 0);
        glutSolidSphere(0.5, 5, 5); glPopMatrix();
    }
    glPopMatrix();
}

void CityMasterPlan::drawGazebo(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.5f, 0.35f, 0.2f); glPushMatrix(); glScalef(30, 1, 30); glutSolidCube(1); glPopMatrix();
    glColor3f(0.4f, 0.2f, 0.1f); float pOff = 13.0f;
    for (int i = 0; i < 4; i++) {
        glPushMatrix(); float px = (i < 2) ? pOff : -pOff; float pz = (i % 2 == 0) ? pOff : -pOff;
        glTranslatef(px, 10, pz); glScalef(1, 20, 1); glutSolidCube(1); glPopMatrix();
    }
    glColor3f(0.3f, 0.1f, 0.0f);
    glPushMatrix(); glTranslatef(0, 20, 0); glRotatef(-45, 0, 1, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(22, 10, 4, 1); glPopMatrix();
    glPopMatrix();
}

void CityMasterPlan::drawWoodenBridge(float x, float y, float z, float length, float width) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.55f, 0.4f, 0.25f); int numPlanks = 40; float plankW = length / numPlanks;
    for (int i = 0; i < numPlanks; i++) {
        float progress = (float)i / numPlanks; float arcH = sin(progress * M_PI) * 8.0f;
        glPushMatrix(); glTranslatef(-length / 2 + i * plankW, arcH, 0); glScalef(plankW * 0.9, 0.5, width); glutSolidCube(1); glPopMatrix();
        glColor3f(0.3f, 0.2f, 0.1f);
        glPushMatrix(); glTranslatef(-length / 2 + i * plankW, arcH + 3, width / 2 - 0.5); glScalef(0.5, 6, 0.5); glutSolidCube(1); glPopMatrix();
        glPushMatrix(); glTranslatef(-length / 2 + i * plankW, arcH + 3, -width / 2 + 0.5); glScalef(0.5, 6, 0.5); glutSolidCube(1); glPopMatrix();
        glColor3f(0.55f, 0.4f, 0.25f);
    }
    glPopMatrix();
}

void CityMasterPlan::drawParkingCanopy(float x, float y, float z, float width, float length) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.3f, 0.3f, 0.35f); float poleH = 15.0f;
    glPushMatrix(); glTranslatef(-width / 2 + 1, poleH / 2, -length / 2 + 1); glScalef(1, poleH, 1); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(width / 2 - 1, poleH / 2, -length / 2 + 1); glScalef(1, poleH, 1); glutSolidCube(1); glPopMatrix();
    glColor3f(0.9f, 0.9f, 0.95f);
    glBegin(GL_QUADS); glVertex3f(-width / 2, poleH, -length / 2); glVertex3f(width / 2, poleH, -length / 2); glVertex3f(width / 2, poleH - 3, length / 2); glVertex3f(-width / 2, poleH - 3, length / 2); glEnd();
    glPopMatrix();
}

void CityMasterPlan::drawModernBench(float x, float y, float z, float rotation) {
    glPushMatrix(); glTranslatef(x, y, z); glRotatef(rotation, 0, 1, 0); glDisable(GL_TEXTURE_2D);
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix(); glTranslatef(6, 2, 0); glScalef(1, 4, 6); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(-6, 2, 0); glScalef(1, 4, 6); glutSolidCube(1); glPopMatrix();
    glColor3f(0.6f, 0.3f, 0.1f); glPushMatrix(); glTranslatef(0, 4.5f, 0); glScalef(14, 1, 6); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 7.5f, -2.5f); glScalef(14, 4, 1); glutSolidCube(1); glPopMatrix();
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix(); glTranslatef(5, 6, -2.5f); glScalef(0.5, 4, 0.5); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(-5, 6, -2.5f); glScalef(0.5, 4, 0.5); glutSolidCube(1); glPopMatrix();
    glPopMatrix();
}

void CityMasterPlan::drawGardenLamp(float x, float y, float z, bool isOn) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.1f, 0.1f, 0.1f); glPushMatrix(); glScalef(1, 15, 1); glutSolidCube(1); glPopMatrix();
    glTranslatef(0, 8, 0); glColor3f(0.2f, 0.2f, 0.2f); glutSolidSphere(1.5, 10, 10);
    if (isOn) {
        glDisable(GL_LIGHTING); glColor3f(1.0f, 1.0f, 0.8f); glutSolidSphere(1.6, 10, 10);
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glDepthMask(GL_FALSE);
        glPushMatrix(); glRotatef(90, 1, 0, 0); glColor4f(1.0f, 0.9f, 0.5f, 0.2f); glutSolidCone(10.0, 20.0, 20, 10); glPopMatrix();
        glDepthMask(GL_TRUE); glDisable(GL_BLEND); glEnable(GL_LIGHTING);
    }
    else { glColor3f(0.4f, 0.4f, 0.4f); glutSolidSphere(1.6, 10, 10); }
    glPopMatrix();
}

void CityMasterPlan::drawCafeSet(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // تكبير الحجم 3 أضعاف
    glScalef(3.0f, 3.0f, 3.0f);

    glDisable(GL_TEXTURE_2D);

    // الطاولة
    glColor3f(0.9f, 0.9f, 0.9f);
    glPushMatrix(); glTranslatef(0, 3, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(4, 1, 10, 1); glPopMatrix();
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix(); glTranslatef(0, 1.5, 0); glScalef(0.5, 3, 0.5); glutSolidCube(1); glPopMatrix();

    // المظلة (قماش مخطط أحمر وأبيض لواقعية أكثر)
    glPushMatrix(); glTranslatef(0, 12, 0); glRotatef(-90, 1, 0, 0);
    glColor3f(0.8f, 0.2f, 0.2f); glutSolidCone(8, 4, 8, 1); // الجزء الأحمر
    glPopMatrix();

    // عمود المظلة
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix(); glTranslatef(0, 6, 0); glScalef(0.3, 12, 0.3); glutSolidCube(1); glPopMatrix();

    // الكراسي
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(i * 90 + 45, 0, 1, 0);
        glTranslatef(0, 0, 5); // أبعدنا الكراسي قليلاً لتناسب الحجم

        glColor3f(0.4f, 0.2f, 0.1f);
        glPushMatrix(); glTranslatef(0, 2, 0); glScalef(3, 0.5, 3); glutSolidCube(1); glPopMatrix();
        glPushMatrix(); glTranslatef(0, 4, -1.5); glScalef(3, 4, 0.5); glutSolidCube(1); glPopMatrix();
        glPopMatrix();
    }
    glPopMatrix();
}

void CityMasterPlan::drawModernKiosk(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.2f, 0.2f, 0.2f); glPushMatrix(); glTranslatef(0, 6, 0); glScalef(15, 12, 10); glutSolidCube(1); glPopMatrix();
    glColor3f(0.9f, 0.9f, 0.9f); glPushMatrix(); glTranslatef(0, 6, 5.1f); glScalef(12, 6, 0.1f); glutSolidCube(1); glPopMatrix();
    glColor3f(0.6f, 0.4f, 0.2f); glPushMatrix(); glTranslatef(0, 3, 6.0f); glScalef(16, 1, 2); glutSolidCube(1); glPopMatrix();
    glColor3f(0.8f, 0.5f, 0.0f); glPushMatrix(); glTranslatef(0, 12.5f, 0); glScalef(16, 1, 12); glutSolidCube(1); glPopMatrix();
    glPopMatrix();
}
void CityMasterPlan::drawStreetLightRow(float x, float startZ, float spacing, int count, bool isRightSide, bool isNight) {
    for (int i = 0; i < count; i++) {
        glPushMatrix();
        // الموقع
        glTranslatef(x, 0, startZ + i * spacing);

        // التدوير: إذا كان على اليمين يدور باتجاه الطريق (-90)، والعكس
        // StreetLamp عادة يرسم باتجاه Z، لذا ندوره حول Y
        if (isRightSide) glRotatef(-90, 0, 1, 0); // ينظر لليسار (نحو الشارع)
        else glRotatef(90, 0, 1, 0);              // ينظر لليمين (نحو الشارع)

        streetLightObj.draw(isNight);
        glPopMatrix();
    }
}
void CityMasterPlan::drawStreetLampBase(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(0.1f, 0.1f, 0.1f); glScalef(2, 1, 2); glutSolidCube(1);
    glPopMatrix();
}

// --- دالة رسم كشك العصير (تمت إضافتها هنا) ---
void CityMasterPlan::drawJuiceStall(float x, float y, float z) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 0.5f, 0.0f); glPushMatrix(); glTranslatef(0, 5, 0); glScalef(12, 10, 12); glutSolidCube(1); glPopMatrix();
    glColor3f(0.0f, 0.8f, 0.0f); glPushMatrix(); glTranslatef(0, 11, 6); glScalef(10, 2, 1); glutSolidCube(1); glPopMatrix();
    glColor3f(1, 1, 0); glPushMatrix(); glTranslatef(-3, 6, 6.5); glutSolidTeapot(1); glPopMatrix();
    glColor3f(1, 0, 1); glPushMatrix(); glTranslatef(3, 6, 6.5); glutSolidTeapot(1); glPopMatrix();
    glPopMatrix();
}

void CityMasterPlan::drawFence(float w, float l) {
    glDisable(GL_TEXTURE_2D); glColor3f(0.2f, 0.2f, 0.2f); float poleH = 8.0f; float gap = 15.0f;
    for (float i = -w / 2; i <= w / 2; i += gap) {
        glPushMatrix(); glTranslatef(i, poleH / 2, -l / 2); glScalef(1, poleH, 1); glutSolidCube(1); glPopMatrix();
        if (abs(i) > 40) { glPushMatrix(); glTranslatef(i, poleH / 2, l / 2); glScalef(1, poleH, 1); glutSolidCube(1); glPopMatrix(); }
    }
    for (float i = -l / 2; i <= l / 2; i += gap) {
        glPushMatrix(); glTranslatef(w / 2, poleH / 2, i); glScalef(1, poleH, 1); glutSolidCube(1); glPopMatrix();
        glPushMatrix(); glTranslatef(-w / 2, poleH / 2, i); glScalef(1, poleH, 1); glutSolidCube(1); glPopMatrix();
    }
}

void CityMasterPlan::drawMainGate() {
    glDisable(GL_TEXTURE_2D); glColor3f(0.8f, 0.7f, 0.6f);
    glPushMatrix(); glTranslatef(40, 12, 0); glScalef(15, 24, 15); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(-40, 12, 0); glScalef(15, 24, 15); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 22, 0); glScalef(100, 6, 10); glutSolidCube(1); glPopMatrix();
    glColor3f(0.1f, 0.4f, 0.1f); glPushMatrix(); glTranslatef(0, 26, 0); glScalef(50, 4, 2); glutSolidCube(1); glPopMatrix();
}

void CityMasterPlan::drawBalloon(float x, float y, float z, float r, float g, float b) {
    glPushMatrix(); glTranslatef(x, y, z); glDisable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1); glBegin(GL_LINES); glVertex3f(0, 0, 0); glVertex3f(0, -10, 0); glEnd();
    glColor3f(r, g, b); glScalef(1, 1.2, 1); glutSolidSphere(2, 10, 10);
    glPopMatrix();
}

void CityMasterPlan::drawGrandOpeningDecor() {
    float startZ = 200.0f; float endZ = 280.0f;
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.8f, 0.0f, 0.0f); // أحمر

    // رفعنا السجادة إلى 0.15f لتكون آمنة تماماً من الوميض
    float carpetY = 0.15f;

    glBegin(GL_QUADS); glNormal3f(0, 1, 0);
    glVertex3f(-15, carpetY, startZ); glVertex3f(15, carpetY, startZ);
    glVertex3f(15, carpetY, endZ); glVertex3f(-15, carpetY, endZ);
    glEnd();

    // البالونات
    for (int i = 0; i < 6; i++) {
        float z = startZ + 10 + i * 12;
        drawBalloon(20, 15 + (i % 2) * 2, z, 1, 0, 0); drawBalloon(22, 12 + (i % 2) * 2, z, 1, 1, 0);
        drawBalloon(-20, 15 + (i % 2) * 2, z, 1, 0, 0); drawBalloon(-22, 12 + (i % 2) * 2, z, 1, 1, 0);
    }
}



// --- لم تعد تستخدم في الحديقة الجديدة ولكن قد تحتاجها للتوافق ---
void CityMasterPlan::drawRock(float x, float y, float z, float size) {}
void CityMasterPlan::drawReed(float x, float y, float z) {}

// ==========================================
// البنية التحتية الأساسية
// ==========================================

void CityMasterPlan::drawInfiniteGround() {
    // 1. تفعيل الـ Stencil لمنع رسم العشب تحت البحيرة (كما هو)
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); glDepthMask(GL_FALSE);

    float lakeX = -280.0f; float lakeZ = 600.0f; float lakeW = 200.0f; float lakeL = 300.0f;
    glBegin(GL_QUADS); glVertex3f(lakeX - lakeW / 2, -0.5f, lakeZ - lakeL / 2); glVertex3f(lakeX + lakeW / 2, -0.5f, lakeZ - lakeL / 2);
    glVertex3f(lakeX + lakeW / 2, -0.5f, lakeZ + lakeL / 2); glVertex3f(lakeX - lakeW / 2, -0.5f, lakeZ + lakeL / 2); glEnd();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); glDepthMask(GL_TRUE);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // 2. إعدادات الخامة الاحترافية (Material Settings)
    // نجعل العشب "مطفأ" (Matte) لا يعكس ضوء الشمس القوي
    GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // لا لمعان
    GLfloat mat_shininess[] = { 0.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // 3. الرسم
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTex);

    // تحسين جودة التكرار
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // لون أبيض كامل ليظهر لون الصورة الحقيقي
    glColor3f(1.0f, 1.0f, 1.0f);

    float size = 6000.0f; // مساحة ضخمة جداً لتغطية الأفق
    float y = -0.5f;

    // التكرار الاحترافي: 0.1 يعني الصورة تتكرر كل 10 وحدات
    // هذا يجعل العشب يبدو دقيقاً جداً
    float scaleFactor = 0.1f;

    glBegin(GL_QUADS); glNormal3f(0, 1, 0);
    glTexCoord2f(0.0f, 0.0f);                   glVertex3f(-size, y, -size);
    glTexCoord2f(size * scaleFactor, 0.0f);     glVertex3f(size, y, -size);
    glTexCoord2f(size * scaleFactor, size * scaleFactor); glVertex3f(size, y, size);
    glTexCoord2f(0.0f, size * scaleFactor);     glVertex3f(-size, y, size);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // إعادة إعدادات اللمعان الافتراضية لباقي المجسمات (السيارات وغيرها)
    GLfloat default_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat default_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, default_shininess);

    glDisable(GL_STENCIL_TEST);
}


void CityMasterPlan::drawRoadMarkings(float x, float y, float z, float width, float length, bool isHorizontal) {
    glDisable(GL_TEXTURE_2D); float yL = y + 0.03f; glColor3f(1, 1, 1);
    glBegin(GL_QUADS); if (!isHorizontal) { glVertex3f(x - width / 2 + 1, yL, z); glVertex3f(x - width / 2 + 2, yL, z); glVertex3f(x - width / 2 + 2, yL, z + length); glVertex3f(x - width / 2 + 1, yL, z + length); glVertex3f(x + width / 2 - 2, yL, z); glVertex3f(x + width / 2 - 1, yL, z); glVertex3f(x + width / 2 - 1, yL, z + length); glVertex3f(x + width / 2 - 2, yL, z + length); }
    else { glVertex3f(x, yL, z - width / 2 + 1); glVertex3f(x + length, yL, z - width / 2 + 1); glVertex3f(x + length, yL, z - width / 2 + 2); glVertex3f(x, yL, z - width / 2 + 2); glVertex3f(x, yL, z + width / 2 - 2); glVertex3f(x + length, yL, z + width / 2 - 2); glVertex3f(x + length, yL, z + width / 2 - 1); glVertex3f(x, yL, z + width / 2 - 1); } glEnd();
    glColor3f(1, 0.8, 0); float d = 8, g = 12; glBegin(GL_QUADS); if (!isHorizontal) { for (float c = z;c < z + length;c += d + g) { glVertex3f(x - 0.3, yL, c); glVertex3f(x + 0.3, yL, c); glVertex3f(x + 0.3, yL, c + d); glVertex3f(x - 0.3, yL, c + d); } }
    else { for (float c = x;c < x + length;c += d + g) { glVertex3f(c, yL, z - 0.3); glVertex3f(c + d, yL, z - 0.3); glVertex3f(c + d, yL, z + 0.3); glVertex3f(c, yL, z + 0.3); } } glEnd();
}

void CityMasterPlan::drawRoadSegment(float x, float y, float z, float width, float length, bool isHorizontal) {
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, roadTex); setupRepeatedTexture();
    glColor3f(1, 1, 1); float t = 1.0f / 60.0f; glBegin(GL_QUADS); glNormal3f(0, 1, 0);
    if (!isHorizontal) { float u = width * t, v = length * t; glTexCoord2f(0, 0); glVertex3f(x - width / 2, y, z); glTexCoord2f(u, 0); glVertex3f(x + width / 2, y, z); glTexCoord2f(u, v); glVertex3f(x + width / 2, y, z + length); glTexCoord2f(0, v); glVertex3f(x - width / 2, y, z + length); }
    else { float u = length * t, v = width * t; glTexCoord2f(0, 0); glVertex3f(x, y, z - width / 2); glTexCoord2f(u, 0); glVertex3f(x + length, y, z - width / 2); glTexCoord2f(u, v); glVertex3f(x + length, y, z + width / 2); glTexCoord2f(0, v); glVertex3f(x, y, z + width / 2); }
    glEnd(); glDisable(GL_TEXTURE_2D); drawRoadMarkings(x, y, z, width, length, isHorizontal);
}

void CityMasterPlan::drawSidewalk(float x, float z, float width, float length, float height) {
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, pavementTex); setupRepeatedTexture();
    glColor3f(0.9f, 0.9f, 0.9f); float t = 1.0f / 20.0f; float u = width * t, v = length * t;
    glBegin(GL_QUADS); glNormal3f(0, 1, 0); glTexCoord2f(0, 0); glVertex3f(x - width / 2, height, z - length / 2); glTexCoord2f(u, 0); glVertex3f(x + width / 2, height, z - length / 2); glTexCoord2f(u, v); glVertex3f(x + width / 2, height, z + length / 2); glTexCoord2f(0, v); glVertex3f(x - width / 2, height, z + length / 2); glEnd(); glDisable(GL_TEXTURE_2D);
    glColor3f(0.5f, 0.5f, 0.5f); glBegin(GL_QUAD_STRIP); glVertex3f(x - width / 2, 0, z - length / 2); glVertex3f(x - width / 2, height, z - length / 2); glVertex3f(x + width / 2, 0, z - length / 2); glVertex3f(x + width / 2, height, z - length / 2); glVertex3f(x + width / 2, 0, z + length / 2); glVertex3f(x + width / 2, height, z + length / 2); glVertex3f(x - width / 2, 0, z + length / 2); glVertex3f(x - width / 2, height, z + length / 2); glVertex3f(x - width / 2, 0, z - length / 2); glVertex3f(x - width / 2, height, z - length / 2); glEnd();
}

void CityMasterPlan::drawCurvedSidewalk(float x, float y, float z, float radius, float startAngle, float endAngle, float width) {
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, pavementTex); setupRepeatedTexture(); glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUAD_STRIP); for (int i = 0;i <= 20;i++) {
        float a = startAngle + (endAngle - startAngle) * ((float)i / 20);
        glNormal3f(0, 1, 0); glTexCoord2f((float)i / 20, 0); glVertex3f(x + radius * cos(a), y, z + radius * sin(a));
        glTexCoord2f((float)i / 20, 1); glVertex3f(x + (radius + width) * cos(a), y, z + (radius + width) * sin(a));
    } glEnd(); glDisable(GL_TEXTURE_2D);
}

void CityMasterPlan::drawCrosswalk(float x, float y, float z, float width, float length, bool isHorizontal) {
    glDisable(GL_TEXTURE_2D); glColor3f(1, 1, 1); float s = 3, g = 3; glBegin(GL_QUADS);
    if (!isHorizontal) for (float c = z;c < z + length;c += s + g) { glVertex3f(x - width / 2, y + 0.04, c); glVertex3f(x + width / 2, y + 0.04, c); glVertex3f(x + width / 2, y + 0.04, c + s); glVertex3f(x - width / 2, y + 0.04, c + s); }
    else for (float c = x;c < x + length;c += s + g) { glVertex3f(c, y + 0.04, z - width / 2); glVertex3f(c + s, y + 0.04, z - width / 2); glVertex3f(c + s, y + 0.04, z + width / 2); glVertex3f(c, y + 0.04, z + width / 2); } glEnd();
}

void CityMasterPlan::drawParkingLines(float x, float z, float width, float length, int rows, int cols) {
    glColor3f(1, 1, 1); glLineWidth(2); glBegin(GL_LINES);
    // رفعنا الخطوط إلى 0.18f (فوق الأسفلت الذي هو 0.15f)
    float lineY = 0.18f;
    float s = width / cols;
    for (int i = 0; i <= cols; i++) {
        float c = x - width / 2 + i * s;
        glVertex3f(c, lineY, z - length / 2);
        glVertex3f(c, lineY, z + length / 2);
    }
    glEnd();
}



// ==========================================
// المناطق (Zones)
// ==========================================

void CityMasterPlan::drawLakePark(bool isNight) {
    float parkX = -280.0f; float parkZ = 600.0f;
    float parkW = 350.0f; float parkL = 500.0f;
    float lakeW = 200.0f; float lakeL = 300.0f;

    // 1. الرصيف الجامع (الإطار الخارجي فقط مع تفريغ كامل للوسط)
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, pavementTex); setupRepeatedTexture();
    glColor3f(0.9f, 0.9f, 0.9f); float tS = 0.02f;
    glBegin(GL_QUADS); glNormal3f(0, 1, 0);
    // قطعة علوية
    float topH = (parkL - lakeL) / 2.0f; float topZ = parkZ - lakeL / 2 - topH / 2;
    glTexCoord2f(0, 0); glVertex3f(parkX - parkW / 2, 0.05f, topZ - topH / 2); glTexCoord2f(parkW * tS, 0); glVertex3f(parkX + parkW / 2, 0.05f, topZ - topH / 2);
    glTexCoord2f(parkW * tS, topH * tS); glVertex3f(parkX + parkW / 2, 0.05f, topZ + topH / 2); glTexCoord2f(0, topH * tS); glVertex3f(parkX - parkW / 2, 0.05f, topZ + topH / 2);
    // قطعة سفلية
    float botZ = parkZ + lakeL / 2 + topH / 2;
    glTexCoord2f(0, 0); glVertex3f(parkX - parkW / 2, 0.05f, botZ - topH / 2); glTexCoord2f(parkW * tS, 0); glVertex3f(parkX + parkW / 2, 0.05f, botZ - topH / 2);
    glTexCoord2f(parkW * tS, topH * tS); glVertex3f(parkX + parkW / 2, 0.05f, botZ + topH / 2); glTexCoord2f(0, topH * tS); glVertex3f(parkX - parkW / 2, 0.05f, botZ + topH / 2);
    // قطعة يسرى
    float sideW = (parkW - lakeW) / 2.0f; float leftX = parkX - lakeW / 2 - sideW / 2;
    glTexCoord2f(0, 0); glVertex3f(leftX - sideW / 2, 0.05f, parkZ - lakeL / 2); glTexCoord2f(sideW * tS, 0); glVertex3f(leftX + sideW / 2, 0.05f, parkZ - lakeL / 2);
    glTexCoord2f(sideW * tS, lakeL * tS); glVertex3f(leftX + sideW / 2, 0.05f, parkZ + lakeL / 2); glTexCoord2f(0, lakeL * tS); glVertex3f(leftX - sideW / 2, 0.05f, parkZ + lakeL / 2);
    // قطعة يمنى
    float rightX = parkX + lakeW / 2 + sideW / 2;
    glTexCoord2f(0, 0); glVertex3f(rightX - sideW / 2, 0.05f, parkZ - lakeL / 2); glTexCoord2f(sideW * tS, 0); glVertex3f(rightX + sideW / 2, 0.05f, parkZ - lakeL / 2);
    glTexCoord2f(sideW * tS, lakeL * tS); glVertex3f(rightX + sideW / 2, 0.05f, parkZ + lakeL / 2); glTexCoord2f(0, lakeL * tS); glVertex3f(rightX - sideW / 2, 0.05f, parkZ + lakeL / 2);
    glEnd(); glDisable(GL_TEXTURE_2D);

    // 2. السور والبوابة
    glPushMatrix(); glTranslatef(parkX, 0, parkZ); drawFence(parkW, parkL);
    glPushMatrix(); glTranslatef(parkW / 2, 0, 0); glRotatef(-90, 0, 1, 0); drawMainGate(); glPopMatrix();
    glPopMatrix();

    // 3. (تم حذف الجسر والجزيرة وأي رصيف يقطع البحيرة)

    // 4. الكوخ في الزاوية (بعيداً عن الماء)
    drawGazebo(parkX + lakeW / 2 + 40, 0.1f, parkZ - lakeL / 2 + 40);

    // 5. المقاعد والإنارة (حول البحيرة فقط)
    float benchStart = parkZ - 120;
    for (int i = 0; i < 5; i++) {
        float posZ = benchStart + i * 60;
        drawModernBench(parkX + lakeW / 2 + 20, 0.1f, posZ, -90);
        drawGardenLamp(parkX + lakeW / 2 + 35, 0.1f, posZ, isNight);
        drawModernBench(parkX - lakeW / 2 - 20, 0.1f, posZ, 90);
        drawGardenLamp(parkX - lakeW / 2 - 35, 0.1f, posZ, isNight);
    }
    // الأشجار المحيطة
    for (int i = 0; i < 6; i++) {
        drawSimpleTree(parkX - parkW / 2 + 30, 0.1f, parkZ - parkL / 2 + 30 + i * 80);
        drawSimpleTree(parkX + parkW / 2 - 30, 0.1f, parkZ - parkL / 2 + 30 + i * 80);
    }
    // كشك العصير
    drawJuiceStall(parkX - parkW / 2 + 40, 0.1f, parkZ - parkL / 2 + 40);
}


void CityMasterPlan::buildShowroomPlaza() {
    float srMinX = -150.0f; float srMaxX = 150.0f; float srMinZ = -200.0f; float srMaxZ = 200.0f;
    float borderWidth = 30.0f; float frontPlazaDepth = 80.0f;

    // 1. رسم الإطار المحيط (الرصيف) فقط - وترك الوسط فارغاً للأرضية الأصلية
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, pavementTex); setupRepeatedTexture();
    glColor3f(0.9f, 0.9f, 0.9f); float tS = 0.02f;
    glBegin(GL_QUADS); glNormal3f(0, 1, 0);
    // الأمامي (الساحة)
    glTexCoord2f(0, 0); glVertex3f(srMinX - borderWidth, 0.02f, srMaxZ);
    glTexCoord2f(10, 0); glVertex3f(srMaxX + borderWidth, 0.02f, srMaxZ);
    glTexCoord2f(10, 2); glVertex3f(srMaxX + borderWidth, 0.02f, srMaxZ + frontPlazaDepth);
    glTexCoord2f(0, 2); glVertex3f(srMinX - borderWidth, 0.02f, srMaxZ + frontPlazaDepth);
    // اليمين
    glTexCoord2f(0, 0); glVertex3f(srMaxX, 0.02f, srMinZ - borderWidth);
    glTexCoord2f(1, 0); glVertex3f(srMaxX + borderWidth, 0.02f, srMinZ - borderWidth);
    glTexCoord2f(1, 10); glVertex3f(srMaxX + borderWidth, 0.02f, srMaxZ);
    glTexCoord2f(0, 10); glVertex3f(srMaxX, 0.02f, srMaxZ);
    // اليسار
    glTexCoord2f(0, 0); glVertex3f(srMinX - borderWidth, 0.02f, srMinZ - borderWidth);
    glTexCoord2f(1, 0); glVertex3f(srMinX, 0.02f, srMinZ - borderWidth);
    glTexCoord2f(1, 10); glVertex3f(srMinX, 0.02f, srMaxZ);
    glTexCoord2f(0, 10); glVertex3f(srMinX - borderWidth, 0.02f, srMaxZ);
    // الخلف
    glTexCoord2f(0, 0); glVertex3f(srMinX - borderWidth, 0.02f, srMinZ - borderWidth);
    glTexCoord2f(10, 0); glVertex3f(srMaxX + borderWidth, 0.02f, srMinZ - borderWidth);
    glTexCoord2f(10, 1); glVertex3f(srMaxX + borderWidth, 0.02f, srMinZ);
    glTexCoord2f(0, 1); glVertex3f(srMinX - borderWidth, 0.02f, srMinZ);
    glEnd(); glDisable(GL_TEXTURE_2D);

    // 2. زينة الافتتاح (رفع السجادة الحمراء)
    drawGrandOpeningDecor(); // تم تعديل ارتفاعها في الدالة الخاصة بها أدناه

    // 3. التجميل المحيط
    drawFlowerBed(60.0f, 0.02f, srMaxZ + 40.0f); drawFlowerBed(-60.0f, 0.02f, srMaxZ + 40.0f);
    drawSimpleTree(srMaxX + 15, 0.02f, srMaxZ + 15); drawSimpleTree(srMinX - 15, 0.02f, srMaxZ + 15);
    drawSimpleTree(srMaxX + 15, 0.02f, srMinZ - 15); drawSimpleTree(srMinX - 15, 0.02f, srMinZ - 15);
}


void CityMasterPlan::buildMainBoulevard(bool isNight) {
    float startZ = 280.0f; // بداية الطريق بعد الساحة
    float entryWidth = 100.0f; float entryLength = 100.0f; float splitZ = startZ + entryLength;

    // --- الجزء الأول: المدخل ---
    drawRoadSegment(0, 0.05f, startZ, entryWidth, entryLength, false);
    drawSidewalk(entryWidth / 2 + 15, startZ + entryLength / 2, 30, entryLength, 0.5f);
    drawSidewalk(-(entryWidth / 2 + 15), startZ + entryLength / 2, 30, entryLength, 0.5f);

    // أشجار المدخل
    for (int i = 0; i < 3; i++) {
        drawSimpleTree(entryWidth / 2 + 10, 0.5f, startZ + 20 + i * 40);
        drawSimpleTree(-(entryWidth / 2 + 10), 0.5f, startZ + 20 + i * 40);
    }

    // --- الجزء الثاني: ما قبل الدوار ---
    float roundaboutCenterZ = 600.0f; float roadLen = roundaboutCenterZ - splitZ;
    float laneWidth = 40.0f; float medianWidth = 20.0f;

    drawRoadSegment(medianWidth / 2 + laneWidth / 2, 0.05f, splitZ, laneWidth, roadLen - 80, false);
    drawRoadSegment(-(medianWidth / 2 + laneWidth / 2), 0.05f, splitZ, laneWidth, roadLen - 80, false);

    // الجزيرة الوسطية والأرصفة
    drawSidewalk(0, splitZ + (roadLen - 80) / 2, medianWidth, roadLen - 80, 0.5f);
    for (int i = 0; i < 5; i++) drawSimpleTree(0, 0.5f, splitZ + 20 + i * 40);
    drawSidewalk(medianWidth / 2 + laneWidth + 10, splitZ + (roadLen - 80) / 2, 20, roadLen - 80, 0.5f);
    drawSidewalk(-(medianWidth / 2 + laneWidth + 10), splitZ + (roadLen - 80) / 2, 20, roadLen - 80, 0.5f);

    // >>> إضافة إنارة الشارع (الجزء الجنوبي) <<<
    // نضع الأعمدة على الأرصفة الجانبية
    float lampX = medianWidth / 2 + laneWidth + 10; // منتصف الرصيف الجانبي
    // يمين
    drawStreetLightRow(lampX, splitZ + 20, 60.0f, 3, true, isNight);
    // يسار
    drawStreetLightRow(-lampX, splitZ + 20, 60.0f, 3, false, isNight);


    // --- الجزء الثالث: ما بعد الدوار (الممتد للأفق) ---
    float northStart = roundaboutCenterZ + (80 + 40); // 720
    float northLength = 1500.0f; // مسافة طويلة

    drawRoadSegment(medianWidth / 2 + laneWidth / 2, 0.05f, northStart, laneWidth, northLength, false);
    drawRoadSegment(-(medianWidth / 2 + laneWidth / 2), 0.05f, northStart, laneWidth, northLength, false);

    // الجزيرة الوسطية الطويلة
    drawSidewalk(0, northStart + northLength / 2, medianWidth, northLength, 0.5f);

    // الأرصفة الجانبية الطويلة
    drawSidewalk(medianWidth / 2 + laneWidth + 10, northStart + northLength / 2, 20, northLength, 0.5f);
    drawSidewalk(-(medianWidth / 2 + laneWidth + 10), northStart + northLength / 2, 20, northLength, 0.5f);

    // أشجار النخيل الممتدة
    for (int i = 0; i < 20; i++) {
        drawSimpleTree(0, 0.5f, northStart + 50 + i * 80);
    }

    // >>> إضافة إنارة الشارع (الجزء الشمالي الطويل) <<<
    // نوزع الأعمدة كل 80 وحدة
    drawStreetLightRow(lampX, northStart + 40, 80.0f, 18, true, isNight);
    drawStreetLightRow(-lampX, northStart + 40, 80.0f, 18, false, isNight);

    // ممرات المشاة
    drawCrosswalk(medianWidth / 2 + laneWidth / 2, 0.05f, startZ + 10, laneWidth, 15.0f, false);
    drawCrosswalk(-(medianWidth / 2 + laneWidth / 2), 0.05f, startZ + 10, laneWidth, 15.0f, false);
    drawCrosswalk(medianWidth / 2 + laneWidth / 2, 0.05f, 550.0f, laneWidth, 15.0f, false);
    drawCrosswalk(-(medianWidth / 2 + laneWidth / 2), 0.05f, 550.0f, laneWidth, 15.0f, false);
}

void CityMasterPlan::drawRoundabout(float x, float y, float z, float radius, float roadWidth) {
    glPushMatrix(); glTranslatef(x, y, z);
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, roadTex); setupRepeatedTexture();
    glColor3f(1, 1, 1);
    int segments = 64; glBegin(GL_QUAD_STRIP); for (int i = 0; i <= segments; i++) { float a = (float)i / segments * 2 * M_PI; float rO = radius + roadWidth; glNormal3f(0, 1, 0); glTexCoord2f(0, (float)i / 5); glVertex3f(radius * cos(a), 0.05, radius * sin(a)); glTexCoord2f(1, (float)i / 5); glVertex3f(rO * cos(a), 0.05, rO * sin(a)); } glEnd(); glDisable(GL_TEXTURE_2D);
    float islandR = radius - 2.0f; glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, pavementTex); setupRepeatedTexture(); glBegin(GL_TRIANGLE_FAN); glNormal3f(0, 1, 0); glTexCoord2f(0.5, 0.5); glVertex3f(0, 0.5, 0); for (int i = 0; i <= segments; i++) { float a = (float)i / segments * 2 * M_PI; glTexCoord2f(0.5 + 0.5 * cos(a), 0.5 + 0.5 * sin(a)); glVertex3f(islandR * cos(a), 0.5, islandR * sin(a)); } glEnd(); glDisable(GL_TEXTURE_2D);
    glColor3f(0.8f, 0.8f, 0.8f); glPushMatrix(); glScalef(10, 20, 10); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 15, 0); glutSolidSphere(8, 20, 20); glPopMatrix();
    drawFlowerBed(15, 0.5f, 0); drawFlowerBed(-15, 0.5f, 0); drawFlowerBed(0, 0.5f, 15); drawFlowerBed(0, 0.5f, -15);
    float outerR = radius + roadWidth;
    drawCurvedSidewalk(outerR + 10, 0.5f, outerR + 10, outerR, M_PI, 1.5 * M_PI, 20);
    drawCurvedSidewalk(-(outerR + 10), 0.5f, outerR + 10, outerR, 1.5 * M_PI, 2 * M_PI, 20);
    drawCurvedSidewalk(outerR + 10, 0.5f, -(outerR + 10), outerR, 0.5 * M_PI, M_PI, 20);
    drawCurvedSidewalk(-(outerR + 10), 0.5f, -(outerR + 10), outerR, 0, 0.5 * M_PI, 20);
    glPopMatrix();
}

void CityMasterPlan::buildParkingZone() {
    float pX = 350.0f; float pZ = 600.0f; float pW = 300.0f; float pL = 400.0f; float border = 40.0f;

    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, pavementTex); setupRepeatedTexture();
    glColor3f(0.9f, 0.9f, 0.9f); float tS = 0.02f; float totalW = pW + border * 2; float totalL = pL + border * 2;
    glBegin(GL_QUADS); glNormal3f(0, 1, 0); glTexCoord2f(0, 0); glVertex3f(pX - totalW / 2, 0.02f, pZ - totalL / 2); glTexCoord2f(totalW * tS, 0); glVertex3f(pX + totalW / 2, 0.02f, pZ - totalL / 2); glTexCoord2f(totalW * tS, totalL * tS); glVertex3f(pX + totalW / 2, 0.02f, pZ + totalL / 2); glTexCoord2f(0, totalL * tS); glVertex3f(pX - totalW / 2, 0.02f, pZ + totalL / 2); glEnd(); glDisable(GL_TEXTURE_2D);

    float entryW = 80.0f; drawSidewalk((150.0f + (pX - pW / 2)) / 2, pZ, (pX - pW / 2) - 150.0f, entryW, 0.025f);

    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, parkingTex); setupRepeatedTexture();
    glColor3f(0.85f, 0.85f, 0.85f); tS = 1.0f / 40.0f;
    glBegin(GL_QUADS); glNormal3f(0, 1, 0); glTexCoord2f(0, 0); glVertex3f(pX - pW / 2, 0.15f, pZ - pL / 2); glTexCoord2f(pW * tS, 0); glVertex3f(pX + pW / 2, 0.15f, pZ - pL / 2); glTexCoord2f(pW * tS, pL * tS); glVertex3f(pX + pW / 2, 0.15f, pZ + pL / 2); glTexCoord2f(0, pL * tS); glVertex3f(pX - pW / 2, 0.15f, pZ + pL / 2); glEnd(); glDisable(GL_TEXTURE_2D);

    int numRows = 3; float rowHeight = pL / numRows;
    for (int i = 0; i < numRows; i++) {
        float cZ = (pZ - pL / 2) + rowHeight * i + rowHeight / 2;
        if (i < numRows - 1) {
            drawSidewalk(pX, cZ + rowHeight / 2, pW - 20, 8.0f, 0.20f);
            drawSimpleTree(pX - 80, 0.20f, cZ + rowHeight / 2);
            drawSimpleTree(pX + 80, 0.20f, cZ + rowHeight / 2);
            drawGardenLamp(pX, 0.20f, cZ + rowHeight / 2, false);
        }

        // رسم خطوط لمواقف أقل وأعرض
        // عرض الموقف 40 (بدلاً من 20)
        int spots = 7; // 7 مواقف فقط
        float sW = (pW - 40) / spots;
        glColor3f(1, 1, 1); glLineWidth(2); glBegin(GL_LINES);
        for (int k = 0; k <= spots; k++) { float cx = (pX - (pW - 40) / 2) + k * sW; glVertex3f(cx, 0.18f, cZ - (rowHeight - 20) / 2); glVertex3f(cx, 0.18f, cZ + (rowHeight - 20) / 2); } glEnd();

        drawParkingCanopy(pX, 0.10f, cZ - (rowHeight - 20) / 2 + 2, pW - 40, 5.0f);

        // >>> توزيع السيارات (5 سيارات في الصف فقط) <<<
        for (int k = 0; k < spots; k++) {
            int randSeed = (int)(i * 99 + k * 17);
            if (randSeed % 3 == 0) continue;

            float carX = (pX - (pW - 40) / 2) + k * sW + sW / 2;

            // رفعنا Z لتعويض حجم السيارة
            createAndDrawRandomCar(carX, 0.15f, cZ - (rowHeight - 20) / 2 + 15.0f, 90.0f, randSeed);

            if ((randSeed + 1) % 3 != 0) {
                createAndDrawRandomCar(carX, 0.15f, cZ + (rowHeight - 20) / 2 - 15.0f, -90.0f, randSeed + 7);
            }
        }
    }
    float perimX = pW / 2 + border / 2; float perimZ = pL / 2 + border / 2;
    for (float z = -perimZ; z <= perimZ; z += 60) { drawSimpleTree(pX + perimX, 0.02f, pZ + z); drawGardenLamp(pX + perimX, 0.02f, pZ + z + 30, false); if (abs(z) > 50) drawSimpleTree(pX - perimX, 0.02f, pZ + z); }
    for (float x = -perimX; x <= perimX; x += 60) { drawSimpleTree(pX + x, 0.02f, pZ - perimZ); drawSimpleTree(pX + x, 0.02f, pZ + perimZ); }
}

void CityMasterPlan::createAndDrawRandomCar(float x, float y, float z, float rotation, int seed) {
    int type = seed % 10;

    // ألوان عشوائية
    float r = (seed % 10) / 10.0f;
    float g = ((seed * 3) % 10) / 10.0f;
    float b = ((seed * 7) % 10) / 10.0f;
    if (r < 0.2 && g < 0.2 && b < 0.2) r = 0.8f; // تجنب الأسود الكامل

    Car* car = nullptr;

    if (type < 4) {
        car = new Sedan(x, y, z, rotation, r, g, b);
    }
    else if (type < 6) {
        car = new SUV(x, y, z, rotation, r, g, b);
    }
    else if (type < 8) {
        car = new SportsCar(x, y, z, rotation, 1.0f, 0.1f, 0.1f);
    }
    else if (type == 8) {
        car = new Taxi(x, y, z, rotation);
    }
    else {
        car = new Ambulance(x, y, z, rotation);
    }

    if (car) {
        car->draw();
        delete car; // تنظيف الذاكرة فوراً (لأننا نرسم في كل فريم)
    }
}

void CityMasterPlan::buildCommercialZone() {
    float cX = -350.0f; float cZ = 200.0f; float cW = 300.0f; float cL = 400.0f;

    drawSidewalk(cX, cZ, cW, cL, 0.2f);

    // توزيع متباعد (2x2 بدلاً من 3x3) لتستوعب الطاولات الكبيرة
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            float tx = cX - cW / 4 + col * (cW / 2);
            float tz = cZ - cL / 4 + row * (cL / 2);
            drawCafeSet(tx, 0.2f, tz);
        }
    }

    drawModernKiosk(cX - cW / 2 + 30, 0.2f, cZ + cL / 2 - 30);
    drawModernKiosk(cX + cW / 2 - 30, 0.2f, cZ + cL / 2 - 30);
    drawFlowerBed(cX, 0.2f, cZ);
}

void CityMasterPlan::drawText3D(const char* text, float x, float y, float z, float scale, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // حساب العرض للتوسيط
    float width = 0;
    for (const char* c = text; *c; c++) {
        width += glutStrokeWidth(GLUT_STROKE_ROMAN, *c);
    }

    glScalef(scale, scale, scale);
    glTranslatef(-width / 2.0f, 0, 0);

    // سماكة الخط
    glLineWidth(3.0f);

    // رسم النص عدة مرات لعمل عمق (3D)
    for (int i = 0; i < 8; i++) { // زيادة الطبقات لعمق أكبر
        glPushMatrix();
        glTranslatef(0, 0, i * 1.5f); // إزاحة في العمق

        // الطبقة الأمامية (الوجه)
        if (i == 7) {
            glColor3f(r, g, b); // اللون المطلوب (الأزرق الفاتح)
        }
        // الجوانب (العمق) - نجعلها أغمق قليلاً لتبين التجسيم
        else {
            glColor3f(r * 0.6f, g * 0.6f, b * 0.6f);
        }

        for (const char* c = text; *c; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
    }

    glLineWidth(1.0f);
    glPopMatrix();
}

void CityMasterPlan::drawShowroomSign(bool isNight) {
    float gateZ = 200.0f;
    float signY = 100.0f;

    glPushMatrix();
    glTranslatef(0, signY, gateZ + 3.0f);
    glDisable(GL_TEXTURE_2D);

    // 1. اللوحة الخلفية
    glColor3f(0.8f, 0.8f, 0.85f); // إطار
    glPushMatrix(); glScalef(140, 20, 2); glutSolidCube(1); glPopMatrix();

    glColor3f(0.05f, 0.05f, 0.1f); // خلفية سوداء
    // الخلفية عند Z = 0.5
    glPushMatrix(); glTranslatef(0, 0, 0.5f); glScalef(135, 17, 1.5f); glutSolidCube(1); glPopMatrix();

    // 2. النص (تم تعديل الإزاحة Z)
    if (isNight) {
        glDisable(GL_LIGHTING);
        // تم تغيير Z من -3.0f إلى 4.0f ليكون بارزاً بوضوح أمام اللوحة السوداء
        drawText3D("CARS SHOWROOM", 0, -3.0f, 4.0f, 0.12f, 0.2f, 0.8f, 1.0f);
        glEnable(GL_LIGHTING);
    }
    else {
        // تم تغيير Z من -3.0f إلى 4.0f
        drawText3D("CARS SHOWROOM", 0, -3.0f, 4.0f, 0.12f, 0.0f, 0.0f, 0.8f);
    }

    // 3. شريط إضاءة
    if (isNight) glDisable(GL_LIGHTING);
    glColor3f(0.0f, 0.4f, 1.0f);
    // الشريط أيضاً يجب أن يكون بارزاً (Z=2.0)
    glPushMatrix(); glTranslatef(0, -7.0f, 3.0f); glScalef(130, 0.5f, 0.5f); glutSolidCube(1); glPopMatrix();
    if (isNight) glEnable(GL_LIGHTING);

    glPopMatrix();
}


void CityMasterPlan::drawCityLayout(bool isNight) {
    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);

    drawInfiniteGround();
    buildShowroomPlaza();

    drawShowroomSign(isNight);
    // تمرير متغير الليل هنا
    buildMainBoulevard(isNight);

    float roundR = 80.0f; float roadW = 40.0f;
    drawRoundabout(0, 0.055f, 600.0f, roundR, roadW);

    float outerEdge = roundR + roadW;
    drawRoadSegment(outerEdge + 1000.0f / 2, 0.051f, 600.0f, 1000.0f, 60.0f, true);
    drawRoadSegment(-(outerEdge + 1000.0f / 2), 0.051f, 600.0f, 1000.0f, 60.0f, true);

    buildParkingZone();
    buildCommercialZone();
    drawLakePark(isNight);

    glPopAttrib();
}