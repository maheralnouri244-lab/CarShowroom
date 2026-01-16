#ifndef BEETLECAR_H
#define BEETLECAR_H

#include <GL/glut.h>

class BeetleCar {
public:
    BeetleCar();

    // دالة الرسم المحدثة: تستقبل حالة الأبواب كمصفوفة
    void draw(float x, float y, float z, float scale, float rotateY, int colorIndex, bool isDriving, float doorAngles[]);

private:
    const float* currentBodyColor;

    // دوال مساعدة خاصة (Private Helpers)
    void setMaterial(const float* color, float shine = 0.0f, float alphaOverride = -1.0f);
    void drawDisk(float radius, int slices);
    void drawCylinder(float radius, float height, int slices);

    // أجزاء السيارة
    void drawWhitewallWheel();
    void drawSeat();
    void drawGenericDoor(bool isRight, bool hasMirror);
    void drawExtendedChassis();
    void drawOpenCabin();
    void drawDetails();
    void drawFrontEnd();
    void drawGlass();
    void drawHands();
};

#endif
