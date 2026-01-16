// ========================================================================
// OpenGL Architect: GELILI
// Asset: The Autonomous Jeep (Self-Contained & Protected)
// ========================================================================

#include <GL/glut.h>
#include <cmath>
#include <vector>

// --- PROTECTION MACROS ---
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

// --- RENAMED CLASS 1: WHEEL ---
class Jeep_Wheel_System {
private:
    float radius, width;
    void drawCylinderInternal(float r, float w) {
        GLUquadricObj* q = gluNewQuadric();
        gluCylinder(q, r, r, w, 32, 1);
        glPushMatrix(); glRotatef(180, 1, 0, 0); gluDisk(q, 0, r, 32, 1); glPopMatrix();
        glPushMatrix(); glTranslatef(0, 0, w); gluDisk(q, 0, r, 32, 1); glPopMatrix();
        gluDeleteQuadric(q);
    }
public:
    Jeep_Wheel_System(float r, float w) { radius = r; width = w; }
    void render(float x, float y, float z, bool isRightSide, float spin) {
        glPushMatrix();
        glTranslatef(x, y, z);
        if (isRightSide) glRotatef(90.0f, 0, 1, 0); else glRotatef(-90.0f, 0, 1, 0);
        glRotatef(-spin, 0, 0, 1);
        GLfloat rub[] = { 0.15f,0.15f,0.15f,1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, rub);
        drawCylinderInternal(radius, width);
        GLfloat rim[] = { 0.2f,0.2f,0.2f,1.0f }; GLfloat spec[] = { 0.5f,0.5f,0.5f,1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, rim); glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
        glPushMatrix(); glTranslatef(0, 0, width + 0.005f);
        GLUquadricObj* q = gluNewQuadric(); gluDisk(q, 0, radius * 0.75, 7, 1); gluDeleteQuadric(q);
        GLfloat sil[] = { 0.9f,0.9f,0.95f,1.0f }; GLfloat shine[] = { 1.0f,1.0f,1.0f,1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, sil); glMaterialfv(GL_FRONT, GL_SPECULAR, shine); glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);
        for (int i = 0; i < 7; i++) {
            glPushMatrix(); glRotatef(i * 51.4f, 0, 0, 1); glTranslatef(radius * 0.5, 0, 0.02); glutSolidSphere(0.035, 10, 10); glPopMatrix();
        }
        glPopMatrix(); glPopMatrix();
    }
};

// --- RENAMED CLASS 2: GLASS (Was Maher) ---
class Jeep_Glass_System {
private:
    GLuint envTexID;
public:
    void init() {
        const int s = 64; // Low Res is enough for reflection
        unsigned char d[s][s][3];
        for (int y = 0; y < s; y++) {
            float t = y / (float)s;
            for (int x = 0; x < s; x++) {
                if (t < 0.5f) { d[y][x][0] = 60; d[y][x][1] = 60; d[y][x][2] = 65; }
                else { d[y][x][0] = 200; d[y][x][1] = 220; d[y][x][2] = 255; }
            }
        }
        glGenTextures(1, &envTexID);
        glBindTexture(GL_TEXTURE_2D, envTexID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s, s, 0, GL_RGB, GL_UNSIGNED_BYTE, d);
    }
    void enableGlassEffect() {
        // Force enable texture for glass specifically
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, envTexID);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

        GLfloat glassDiffuse[] = { 0.1f, 0.2f, 0.3f, 0.55f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glassDiffuse);
        glDepthMask(GL_FALSE);
    }
    void disableGlassEffect() {
        glDepthMask(GL_TRUE);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_BLEND);
        // We disable texture here because the rest of the car doesn't need it
        glDisable(GL_TEXTURE_2D);
    }
};

// --- RENAMED CLASS 3: DETAILS (Was Abrar) ---
class Jeep_Detail_System {
public:
    void init() {}
    void drawRoofSafariLights(float roofWidth, float x, float y, float z) {
        glPushMatrix(); glTranslatef(x, y, z);
        GLfloat barColor[] = { 0.05f, 0.05f, 0.05f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, barColor);
        glPushMatrix(); glScalef(roofWidth * 0.9f, 0.06f, 0.06f); glutSolidCube(1.0f); glPopMatrix();
        int numLights = 5; float usableWidth = roofWidth * 0.8f; float spacing = usableWidth / (numLights - 1); float startX = -usableWidth / 2.0f;
        for (int i = 0; i < numLights; i++) {
            float lightX = startX + (i * spacing);
            glPushMatrix(); glTranslatef(lightX, 0.08f, 0.0f);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, barColor); glPushMatrix(); glScalef(1.0f, 1.0f, 1.2f); glutSolidSphere(0.12f, 16, 16); glPopMatrix();
            GLfloat lensOff[] = { 0.9f, 0.9f, 0.7f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, lensOff);
            glPushMatrix(); glTranslatef(0.0f, 0.0f, -0.1f); glScalef(1.0f, 1.0f, 0.2f); glutSolidSphere(0.10f, 12, 12); glPopMatrix();
            glPopMatrix();
        }
        glPopMatrix();
    }
    void drawSideMirror(float x, float y, float z, bool isRight, float foldAngle) {
        glPushMatrix(); glTranslatef(x, y, z);
        float baseAngle = isRight ? -15.0f : 15.0f; float dynamicFold = isRight ? foldAngle : -foldAngle;
        glRotatef(baseAngle + dynamicFold, 0, 1, 0);
        GLfloat plasticColor[] = { 0.1f, 0.1f, 0.1f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, plasticColor);
        glPushMatrix(); glRotatef(isRight ? 20.0f : -20.0f, 0, 0, 1); glScalef(0.2f, 0.08f, 0.08f); glutSolidCube(1.0f); glPopMatrix();
        float dir = isRight ? 1.0f : -1.0f; glTranslatef(dir * 0.25f, 0.1f, 0.0f);
        glPushMatrix(); glRotatef(isRight ? 10.0f : -10.0f, 0, 1, 0); glScalef(0.35f, 0.45f, 0.15f); glutSolidCube(1.0f);
        GLfloat mirrorFace[] = { 0.8f, 0.85f, 0.9f, 1.0f }; GLfloat highSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mirrorFace); glMaterialfv(GL_FRONT, GL_SPECULAR, highSpec); glMaterialf(GL_FRONT, GL_SHININESS, 120.0f);
        glTranslatef(0.0f, 0.0f, 0.51f); glScalef(0.85f, 0.85f, 0.1f); glutSolidCube(1.0f);
        glPopMatrix(); glPopMatrix();
    }
    void drawDoorPanel(float width, float height, float thickness, bool isRight) {
        glPushAttrib(GL_LIGHTING_BIT);
        glPushMatrix();
        GLfloat doorColor[] = { 0.05f, 0.05f, 0.05f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, doorColor);
        glScalef(thickness, height, width); glutSolidCube(1.0f); glPopMatrix();
        GLfloat contrastWhite[] = { 0.9f, 0.9f, 0.9f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, contrastWhite);
        float hingeZ_Top = -width * 0.35f; float hingeX = isRight ? (thickness / 2.0f + 0.02f) : -(thickness / 2.0f + 0.02f);
        glPushMatrix(); glTranslatef(hingeX, height * 0.3f, hingeZ_Top); glScalef(0.05f, 0.05f, 0.08f); glutSolidCube(1.0f); glPopMatrix();
        glPushMatrix(); glTranslatef(hingeX, -height * 0.3f, hingeZ_Top); glScalef(0.05f, 0.05f, 0.08f); glutSolidCube(1.0f); glPopMatrix();
        float handleZ = width * 0.25f; glPushMatrix(); glTranslatef(hingeX, 0.15f, handleZ); glScalef(0.04f, 0.03f, 0.15f); glutSolidCube(1.0f); glPopMatrix();
        glPopAttrib();
    }
    void drawTailLight(float x, float y, float z) {
        glPushMatrix(); glTranslatef(x, y, z);
        GLfloat black[] = { 0.1f, 0.1f, 0.1f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, black); glScalef(0.35f, 0.30f, 0.15f); glutSolidCube(1.0f);
        GLfloat redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, redLight);
        glTranslatef(0.0f, 0.0f, 0.05f); glScalef(0.8f, 0.8f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
    }
    void drawBumper(float x, float y, float z, float width) {
        glPushMatrix(); glTranslatef(x, y, z);
        GLfloat mat[] = { 0.1f, 0.1f, 0.1f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
        glScalef(width, 0.25f, 0.35f); glutSolidCube(1.0f); glPopMatrix();
    }
    void drawWinch(float x, float y, float z) {
        glPushMatrix(); glTranslatef(x, y, z);
        GLfloat darkGrey[] = { 0.3f, 0.3f, 0.3f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, darkGrey);
        glScalef(0.4f, 0.15f, 0.2f); glutSolidCube(1.0f); glPopMatrix();
    }
    void drawGrille(float width, float height) {
        GLfloat blackMat[] = { 0.1f, 0.1f, 0.1f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, blackMat);
        float slotWidth = width / 15.0f; float startX = -width / 2.0f + slotWidth * 2.0f;
        for (int i = 0; i < 7; i++) { glPushMatrix(); glTranslatef(startX + (i * slotWidth * 2.0f), 0, 0); glScalef(slotWidth, height * 0.8f, 0.05f); glutSolidCube(1.0f); glPopMatrix(); }
    }
    void drawHeadlights(float x, float y, float z) {
        glPushMatrix(); glTranslatef(x, y, z);
        GLfloat housingColor[] = { 0.15f, 0.15f, 0.15f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, housingColor);
        glPushMatrix(); glTranslatef(0.0f, 0.0f, -0.05f); glutSolidTorus(0.04f, 0.24f, 16, 24); glPopMatrix();
        GLfloat lightColor[] = { 1.0f, 1.0f, 0.9f, 1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, lightColor);
        glScalef(1.0f, 1.0f, 0.5f); glutSolidSphere(0.22f, 24, 24); glPopMatrix();
    }
    void drawFender(float x, float y, float z, bool isRight) {
        glPushMatrix(); glTranslatef(x, y, z);
        GLfloat c[] = { 0.15f,0.15f,0.15f,1.0f }; glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
        glScalef(0.85f, 0.08f, 1.3f); glutSolidCube(1.0f); glPopMatrix();
    }
};

// --- RENAMED CLASS 4: BUILDER (Was Hamwi) ---
// >> THIS IS WHERE THE MAGIC FIX IS EMBEDDED <<
class Jeep_Builder_Final {
private:
    Jeep_Wheel_System* wheelModel;
    Jeep_Wheel_System* spareTire;
    Jeep_Detail_System* detailer;
    Jeep_Glass_System* glassMaster;

    float COWL_X_EXPANSION = 0.15f;
    float COWL_Y_OFFSET = 0.0f;
    float COWL_Z_OFFSET = 0.04f;
    float REAR_BUMPER_HEIGHT = -0.4f;
    float FRONT_BUMPER_HEIGHT = -0.1f;
    float WHEEL_FENDER_HEIGHT = 0.1f;

    // Golden Proportions
    const float CAR_LENGTH = 6.3f;
    const float WHEEL_RADIUS = 0.67f;
    const float WHEEL_WIDTH = 0.32f;
    const float CHASSIS_WIDTH = 2.8f;
    const float CHASSIS_CLEARANCE = 1.0f;
    const float TRACK_WIDTH = 2.8f;
    const float CABIN_HEIGHT = 1.25f;

    void drawBox(float w, float h, float d) { glPushMatrix(); glScalef(w, h, d); glutSolidCube(1.0f); glPopMatrix(); }
    void drawWindowPane(float w, float h, float d) { glPushMatrix(); glScalef(w, h, d); glutSolidCube(1.0f); glPopMatrix(); }
    float chassisLen = 5.5f; float chassisHeight = 1.0f; float cabinLen = 3.6f;

    void drawOpaqueBody() {
        GLfloat bodyColor[] = { 0.05f, 0.05f, 0.05f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, bodyColor);
        // Chassis & Body
        glPushMatrix(); glTranslatef(0.0f, 0.0f, 0.0f); drawBox(CHASSIS_WIDTH, chassisHeight, chassisLen); glPopMatrix();
        float doorHeight = chassisHeight * 0.7f; float doorWidth = cabinLen * 0.45f; float doorThickness = 0.15f;
        float doorY = (chassisHeight / 2.0f) - (doorHeight / 2.0f) + 0.2f;
        float backEdge = chassisLen / 2.0f; float cabinZ = backEdge - (cabinLen / 2.0f);
        float doorZ = cabinZ - (cabinLen / 2.0f) + (doorWidth / 2.0f) + 0.2f;
        glPushMatrix(); glTranslatef((CHASSIS_WIDTH / 2.0f) + 0.02f, doorY, doorZ); detailer->drawDoorPanel(doorWidth, doorHeight, doorThickness, true); glPopMatrix();
        glPushMatrix(); glTranslatef(-(CHASSIS_WIDTH / 2.0f) - 0.02f, doorY, doorZ); detailer->drawDoorPanel(doorWidth, doorHeight, doorThickness, false); glPopMatrix();
        float cabinCenterY = (chassisHeight / 2.0f) + (CABIN_HEIGHT / 2.0f);
        glPushMatrix(); glTranslatef(0.0f, cabinCenterY, cabinZ); drawBox(CHASSIS_WIDTH, CABIN_HEIGHT, cabinLen); glPopMatrix();
        float frontEdge = -chassisLen / 2.0f; float windshieldZBase = cabinZ - (cabinLen / 2.0f) - 0.1f;
        float actualPivotZ = windshieldZBase - 0.3f + COWL_Z_OFFSET; float hoodFrontZ = frontEdge + 0.2f;
        float hoodLen = std::abs(actualPivotZ - hoodFrontZ); float hoodCenter = hoodFrontZ + (hoodLen / 2.0f);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, bodyColor);
        glPushMatrix(); glTranslatef(0.0f, 0.1f, hoodCenter); drawBox(CHASSIS_WIDTH * 0.95f, 0.8f, hoodLen); glPopMatrix();
        float baseHeight = 0.3f; float baseY = 0.5f + (baseHeight / 2.0f) - 0.1f + COWL_Y_OFFSET;
        float dynamicSealWidth = CHASSIS_WIDTH + COWL_X_EXPANSION;
        glPushMatrix(); glTranslatef(0.0f, baseY, actualPivotZ); drawBox(dynamicSealWidth, baseHeight, 0.7f); glPopMatrix();
        detailer->drawBumper(0.0f, REAR_BUMPER_HEIGHT, backEdge + 0.15f, CHASSIS_WIDTH + 0.2f);
        detailer->drawBumper(0.0f, FRONT_BUMPER_HEIGHT, frontEdge - 0.25f, CHASSIS_WIDTH + 0.3f);
        detailer->drawWinch(0.0f, FRONT_BUMPER_HEIGHT + 0.2f, frontEdge - 0.25f);
        glPushMatrix(); glTranslatef(0.0f, cabinCenterY, backEdge + (WHEEL_WIDTH / 2.0f) + 0.1f); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); spareTire->render(-0.25f, -0.75f, 0.0f, true, 0.0f); glPopMatrix();
    }

    void drawTransparentParts() {
        glassMaster->enableGlassEffect();
        float cabinCenterY = (chassisHeight / 2.0f) + (CABIN_HEIGHT / 2.0f);
        float windowHeight = CABIN_HEIGHT * 0.75f; float windowY = cabinCenterY + 0.05f; float windowThickness = 0.1f;
        float sideWindowX = (CHASSIS_WIDTH / 2.0f) + 0.02f;
        float backEdge = chassisLen / 2.0f; float cabinZ = backEdge - (cabinLen / 2.0f);
        float cabinFrontZ = cabinZ - (cabinLen / 2.0f); float pillarPosZ = cabinZ + 0.2f;
        float frontWindowLen = std::abs(pillarPosZ - cabinFrontZ) - 0.5f; float frontWindowZ = pillarPosZ - (frontWindowLen / 2.0f) - 0.2f;
        float cabinBackZ = cabinZ + (cabinLen / 2.0f); float rearWindowLen = std::abs(cabinBackZ - pillarPosZ) - 0.4f; float rearWindowZ = pillarPosZ + (rearWindowLen / 2.0f) + 0.2f;
        glPushMatrix(); glTranslatef(sideWindowX, windowY, frontWindowZ); drawWindowPane(windowThickness, windowHeight, frontWindowLen); glPopMatrix();
        glPushMatrix(); glTranslatef(-sideWindowX, windowY, frontWindowZ); drawWindowPane(windowThickness, windowHeight, frontWindowLen); glPopMatrix();
        glPushMatrix(); glTranslatef(sideWindowX, windowY, rearWindowZ); drawWindowPane(windowThickness, windowHeight, rearWindowLen); glPopMatrix();
        glPushMatrix(); glTranslatef(-sideWindowX, windowY, rearWindowZ); drawWindowPane(windowThickness, windowHeight, rearWindowLen); glPopMatrix();
        float rearGlassZ = cabinBackZ + 0.02f;
        glPushMatrix(); glTranslatef(0.0f, windowY, rearGlassZ); drawWindowPane(CHASSIS_WIDTH - 0.4f, windowHeight, windowThickness); glPopMatrix();
        float windshieldZBase = cabinZ - (cabinLen / 2.0f) - 0.1f;
        float actualPivotZ = windshieldZBase - 0.3f + COWL_Z_OFFSET;
        float baseHeight = 0.3f; float baseY = 0.5f + (baseHeight / 2.0f) - 0.1f + COWL_Y_OFFSET;
        float glassPivotY = baseY + (baseHeight / 2.0f);
        float windshieldGlassHeight = CABIN_HEIGHT * 0.8f;
        float dynamicSealWidth = CHASSIS_WIDTH + COWL_X_EXPANSION;
        float slopeAngle = 22.0f;
        glPushMatrix(); glTranslatef(0.0f, glassPivotY, actualPivotZ); glRotatef(slopeAngle, 1, 0, 0); glTranslatef(0.0f, windshieldGlassHeight / 2.0f, 0.0f);
        drawWindowPane(dynamicSealWidth - 0.05f, windshieldGlassHeight, 0.1f); glPopMatrix();
        float rad = slopeAngle * 3.14159f / 180.0f;
        float topOffsetY = windshieldGlassHeight * cos(rad); float topOffsetZ = windshieldGlassHeight * sin(rad);
        float triBaseY = glassPivotY; float triBaseZ = actualPivotZ; float triX = (dynamicSealWidth - 0.05f) / 2.0f; float baseBottomY = baseY - (baseHeight / 2.0f);
        glBegin(GL_TRIANGLES);
        glNormal3f(1.0f, 0.0f, 0.0f); glVertex3f(triX, triBaseY + topOffsetY, triBaseZ + topOffsetZ); glVertex3f(triX, baseBottomY, triBaseZ + topOffsetZ); glVertex3f(triX, triBaseY, triBaseZ);
        glNormal3f(-1.0f, 0.0f, 0.0f); glVertex3f(-triX, triBaseY + topOffsetY, triBaseZ + topOffsetZ); glVertex3f(-triX, baseBottomY, triBaseZ + topOffsetZ); glVertex3f(-triX, triBaseY, triBaseZ);
        glEnd();
        glassMaster->disableGlassEffect();
    }

public:
    float moveAngle = 0.0f;
    float mirrorFoldAngle = 0.0f;

    void init() {
        detailer = new Jeep_Detail_System();
        glassMaster = new Jeep_Glass_System();
        wheelModel = new Jeep_Wheel_System(WHEEL_RADIUS, WHEEL_WIDTH);
        spareTire = new Jeep_Wheel_System(WHEEL_RADIUS, WHEEL_WIDTH);
        detailer->init();
        glassMaster->init();
    }

    // =========================================================
    // THE SELF-PROTECTING DRAW FUNCTION
    // =========================================================
    void drawJeep(float x, float y, float z, float size) {
        // 1. SAVE EVERYTHING (Protect the showroom from us, and us from the showroom)
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        // 2. THE FIX: FORCE DISABLE SHOWROOM TEXTURES
        glDisable(GL_TEXTURE_2D);

        // 3. THE FIX: DISABLE COLOR MATERIAL (So our black/silver materials work)
        glDisable(GL_COLOR_MATERIAL);

        // 4. THE FIX: ENABLE NORMALIZE (So the large scaling doesn't break lighting)
        glEnable(GL_NORMALIZE);

        // 5. Ensure Lighting is ON
        glEnable(GL_LIGHTING);

        // 6. Optional: Turn off extra showroom lights if it's too bright
        // (Uncomment these if the car is still too white)
        // glDisable(GL_LIGHT1); glDisable(GL_LIGHT2); glDisable(GL_LIGHT3);
        // glDisable(GL_LIGHT4); glDisable(GL_LIGHT5); glDisable(GL_LIGHT6); glDisable(GL_LIGHT7);

        // === START DRAWING ===
        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(size, size, size);

        float chassisLenVal = CAR_LENGTH - 1.0f;
        float axleZ_Rear = (chassisLenVal / 2.0f) - 0.65f;
        float axleZ_Front = -(chassisLenVal / 2.0f) + 0.6f;
        float wheelY = WHEEL_RADIUS;

        wheelModel->render(TRACK_WIDTH / 2.0f, wheelY, axleZ_Front, true, moveAngle);
        wheelModel->render(-TRACK_WIDTH / 2.0f, wheelY, axleZ_Front, false, moveAngle);
        wheelModel->render(TRACK_WIDTH / 2.0f, wheelY, axleZ_Rear, true, moveAngle);
        wheelModel->render(-TRACK_WIDTH / 2.0f, wheelY, axleZ_Rear, false, moveAngle);

        glPushMatrix();
        glTranslatef(0.0f, CHASSIS_CLEARANCE + 0.3f, 0.0f);
        drawOpaqueBody();

        float frontFaceZ = -(chassisLenVal / 2.0f) - 0.1f;
        glPushMatrix(); glTranslatef(0.0f, -0.1f, frontFaceZ); detailer->drawGrille(CHASSIS_WIDTH * 0.8f, 0.5f); glPopMatrix();
        float headlightX = CHASSIS_WIDTH * 0.35f;
        detailer->drawHeadlights(headlightX, 0.20f, frontFaceZ - 0.15f);
        detailer->drawHeadlights(-headlightX, 0.20f, frontFaceZ - 0.15f);

        float backEdge = chassisLen / 2.0f; float cabinZ = backEdge - (cabinLen / 2.0f);
        float cabinCenterY = (chassisHeight / 2.0f) + (CABIN_HEIGHT / 2.0f);
        float roofTopY = cabinCenterY + (CABIN_HEIGHT / 2.0f);
        float roofFrontZ = cabinZ - (cabinLen / 2.0f) + 0.1f;
        detailer->drawRoofSafariLights(CHASSIS_WIDTH, 0.0f, roofTopY, roofFrontZ);

        float windshieldZBase = cabinZ - (cabinLen / 2.0f) - 0.1f;
        float actualPivotZ = windshieldZBase - 0.3f + COWL_Z_OFFSET;
        float baseHeight = 0.3f; float baseY = 0.5f + (baseHeight / 2.0f) - 0.1f + COWL_Y_OFFSET;
        float mirrorY = baseY + 0.2f; float mirrorZ = actualPivotZ + 0.25f; float mirrorX = (CHASSIS_WIDTH / 2.0f) + 0.1f;
        detailer->drawSideMirror(mirrorX, mirrorY, mirrorZ, true, mirrorFoldAngle);
        detailer->drawSideMirror(-mirrorX, mirrorY, mirrorZ, false, mirrorFoldAngle);

        float tailLightX = CHASSIS_WIDTH / 2.0f - 0.25f; float tailLightY = REAR_BUMPER_HEIGHT + 0.35f;
        float backEdgeLocal = chassisLenVal / 2.0f + 0.28f;
        detailer->drawTailLight(tailLightX, tailLightY, backEdgeLocal);
        detailer->drawTailLight(-tailLightX, tailLightY, backEdgeLocal);

        float fenderY = WHEEL_FENDER_HEIGHT; float fenderX = TRACK_WIDTH / 2.0f - 0.1f;
        detailer->drawFender(fenderX, fenderY, axleZ_Front, true);
        detailer->drawFender(-fenderX, fenderY, axleZ_Front, false);
        detailer->drawFender(fenderX, fenderY, axleZ_Rear, true);
        detailer->drawFender(-fenderX, fenderY, axleZ_Rear, false);

        drawTransparentParts();
        glPopMatrix();
        glPopMatrix();

        // 7. RESTORE EVERYTHING (As if we were never here)
        glPopAttrib();
    }
};