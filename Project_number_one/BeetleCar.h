#ifndef BEETLECAR_H
#define BEETLECAR_H

#include <GL/glut.h>

class BeetleCar {
public:
    BeetleCar();
    // دالة الرسم تستقبل الإحداثيات، الحجم، الدوران، اللون، حالة القيادة، وزوايا الأبواب
    void draw(float x, float y, float z, float scale, float rotateY, int colorIndex, bool isDriving, float doorAngles[]);

    // (لم نعد بحاجة لهذه الدوال العامة هنا لأن التحكم بالأبواب يتم خارجياً في AbrarCode، لكن يمكن إبقاؤها فارغة للتوافق)
    void toggleDoors();
    void updateAnimation();

private:
    const float* currentBodyColor;

    // دوال مساعدة
    void setMaterial(const float* color, float shine = 0.0f, float alphaOverride = -1.0f);
    void drawDisk(float radius, int slices);
    void drawCylinder(float radius, float height, int slices);

    // أجزاء السيارة
    void drawWhitewallWheel();
    void drawSeat();
    void drawGenericDoor(bool isRight, bool hasMirror);
    void drawExtendedChassis();
    void drawFrontEnd();
    void drawOpenCabin();
    void drawDetails();
    void drawGlass();
    void drawHands();
};

#endif
