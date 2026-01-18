#ifndef NORMALCAR_H
#define NORMALCAR_H

#include <GL/glut.h>

class NormalCar {
public:
    NormalCar();
    // الرسم: إحداثيات، حجم، دوران، رقم اللون، زاوية الأبواب
    void draw(float x, float y, float z, float scale, float rotateY, int colorIndex, float doorAngle);

private:
    void drawBox(float w, float h, float l);
    void drawProfessionalWheel();
    void drawSportRoof();
    void setColor(const float* color);
};

#endif