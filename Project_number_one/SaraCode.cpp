#include "SaraCode.h"

SaraCode::SaraCode() {
    // سنضع السيارات الأربعة على الجانب الأيسر (X = -100)
    // وسنغير قيم Z لتكون كلها "خلف بعضها" بعيداً عن المركز
    // لاحظي أن القيم كلها في جهة واحدة من Z (مثلاً من 50 إلى 170)

    // السيارة 1
    cars.push_back({ -110.0f, 0.0f, 160.0f, 90.0f, 0, 0.0f });
    // السيارة 2
    cars.push_back({ -110.0f, 0.0f, 120.0f, 90.0f, 1, 0.0f });
    // السيارة 3
    cars.push_back({ -110.0f, 0.0f, 80.0f,  90.0f, 2, 0.0f });
    // السيارة 4
    cars.push_back({ -110.0f, 0.0f, 40.0f,  90.0f, 3, 0.0f });
}

void SaraCode::drawAll() {
    for (auto& c : cars) {
        model.draw(c.x, c.y, c.z, 7.0f, c.rotY, c.colorIdx, c.doorAngle);
    }
}

void SaraCode::handleInput(unsigned char key) {
    if (key == 'o' || key == 'O') {
        for (auto& c : cars) if (c.doorAngle < 45.0f) c.doorAngle += 2.0f;
    }
    if (key == 'p' || key == 'P') {
        for (auto& c : cars) if (c.doorAngle > 0.0f) c.doorAngle -= 2.0f;
    }
}